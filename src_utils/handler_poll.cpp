#include "includes.h"


static void	delete_time_wait_state(int sockfd)
{
	char tmp[1];
	ssize_t status = 0;
	errno = 0;
	syslog(LOG_DEBUG, "READ FROM BUFF => %d", sockfd);
	while ((status = recv(sockfd, tmp, 1, MSG_DONTWAIT)) > 0)
	{
		syslog(LOG_DEBUG, "CONTENT READ => %s", tmp);
		bzero(tmp, 1);
	}
	errno = 0;
	syslog(LOG_DEBUG, "CONTENT STATUS => %ld %m", status);
	tmp[0] = 0;
	if ((status = write(sockfd, tmp, 1)) > 0)
	{
		syslog(LOG_DEBUG, "CONTENT WRITE => %s", tmp);
	}

	syslog(LOG_DEBUG, "CONTENT STATUS => %ld %m", status);
	shutdown(sockfd, SHUT_RDWR);
}

void	handler_poll(void *data, int event, poll_t *poll)
{
	struct s_event			*new_data;
	struct s_event			*event_data;
	struct sockaddr_storage	client_addr = {};
	socklen_t				sin_size;
	int						new_socket;
	char 					buff[INET6_ADDRSTRLEN];

	sin_size = sizeof(struct sockaddr_storage);
	event_data = static_cast<struct s_event *>(data);
	if (event_data->fd == event_data->socket->get_socketfd())
	{
		syslog(LOG_DEBUG, "new connection from socket server => %d", event_data->fd);
		if ((new_socket = accept(event_data->fd, (struct sockaddr *)&client_addr, &sin_size)) < 0)
		{
			syslog(LOG_WARNING, "failed to accept new connection %m");
			return ;
		}
		set_nonblocking(new_socket);
		syslog(LOG_INFO, "accept new connection => %d", new_socket);
		new_data = new struct s_event;
		new_data->fd = new_socket;
		new_data->socket = event_data->socket;
#if defined __linux__
		if (poll_add(poll, new_socket, EPOLLIN | EPOLLHUP | EPOLLRDHUP, new_data) != 0)
		{
			syslog(LOG_WARNING, "failed to accept new connection %m");
			delete new_data;
			close(new_socket);
			return ;
		}
#endif
#if defined __APPLE__
		if (poll_add(poll, new_socket, EVFILT_READ, new_data) != 0)
		{
			syslog(LOG_WARNING, "failed to accept new connection %m");
			delete new_data;
			close(new_socket);
			return ;
		}
#endif
		if (client_addr.ss_family == AF_INET)
		{
			inet_ntop(client_addr.ss_family, (const void *)&((struct sockaddr_in *)&client_addr)->sin_addr, buff, INET_ADDRSTRLEN);
			syslog(LOG_INFO, "New connexion %s:%d", buff, ntohs(((struct sockaddr_in *)&client_addr)->sin_port));
		}
		else if (client_addr.ss_family == AF_INET6)
		{
			inet_ntop(client_addr.ss_family, (const void *)&((struct sockaddr_in6 *)&client_addr)->sin6_addr, buff, INET6_ADDRSTRLEN);
			syslog(LOG_INFO, "New connexion %s:%d", buff, ntohs(((struct sockaddr_in6 *)&client_addr)->sin6_port));
		}
	}
#if defined __linux__
	else if (event & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
#endif
#if defined __APPLE__
	else if (event == EV_ERROR)
#endif
	{
		delete_time_wait_state(event_data->fd);

		if (close(event_data->fd) < 0)
			syslog(LOG_DEBUG, "failed to close socket => %d %m", event_data->fd);
		poll_del(poll, event_data->fd);
		event_data->socket->delete_buff(event_data->fd);
		syslog(LOG_INFO, "close socket %d", event_data->fd);
		delete event_data;
	}
#if defined __linux__
	else if (event & EPOLLIN)
#endif
#if defined __APPLE__
	else if (event == EVFILT_READ)
#endif
	{
		int status;
		syslog(LOG_DEBUG, "new buffer to read from socket %d", event_data->fd);
		if ((status = event_data->socket->read_socket(event_data->fd)))
		{
#if defined __linux__
			if (poll_mod(poll, event_data->fd, EPOLLOUT | EPOLLHUP | EPOLLRDHUP, data) != 0 )
				syslog(LOG_WARNING, "failed to modify connexion connection for the socket %d %m", event_data->fd);
#endif
#if defined __APPLE__
			if (poll_mod(poll, event_data->fd, EVFILT_WRITE, data) != 0)
				syslog(LOG_WARNING, "failed to modify connexion connection for the socket %d %m", event_data->fd);
			if (status == 3)
			{
#if defined __linux__
				if (poll_del(poll, event_data->fd) != 0)
				syslog(LOG_WARNING, "failed to delete socket from poll %d %m", event_data->fd);
#endif
#if defined __APPLE__
				if (poll_del(poll, event_data->fd)  != 0)
					syslog(LOG_WARNING, "failed to delete socket from poll %d %m", event_data->fd);
#endif

				delete_time_wait_state(event_data->fd);
				if (close(event_data->fd) < 0)
					syslog(LOG_DEBUG, "failed to close socket => %d %m", event_data->fd);
				poll_del(poll, event_data->fd);
				event_data->socket->delete_buff(event_data->fd);
				syslog(LOG_INFO, "close socket %d", event_data->fd);
				delete event_data;
				event_data = NULL;
			}
#endif
		}
	}

#if defined __linux__
	else if (event & EPOLLOUT)
#endif
#if defined __APPLE__
	else if (event & EVFILT_WRITE)
#endif
	{
		syslog(LOG_DEBUG, "new buffer to write from socket %d", event_data->fd);
		if (event_data->socket->write_socket(event_data->fd))
		{
#if defined __linux__
			if (poll_del(poll, event_data->fd) != 0)
				syslog(LOG_WARNING, "failed to delete socket from poll %d %m", event_data->fd);
#endif
#if defined __APPLE__
			if (poll_del(poll, event_data->fd)  != 0)
					syslog(LOG_WARNING, "failed to delete socket from poll %d %m", event_data->fd);
#endif

			delete_time_wait_state(event_data->fd);
			if (close(event_data->fd) < 0)
				syslog(LOG_DEBUG, "failed to close socket => %d %m", event_data->fd);
			poll_del(poll, event_data->fd);
			event_data->socket->delete_buff(event_data->fd);
			syslog(LOG_INFO, "close socket %d", event_data->fd);
			delete event_data;
			event_data = NULL;
		}
	}
}
