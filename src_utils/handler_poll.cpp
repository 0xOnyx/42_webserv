#include "includes.h"

void	handler_poll(void *data, uint32_t event, poll_t *poll)
{
	struct s_event			*new_data;
	struct s_event			*event_data;
	struct sockaddr_storage	client_addr = {};
	socklen_t				sin_size;
	int						new_socket;
	char 					buff[INET6_ADDRSTRLEN];

	sin_size = sizeof(struct sockaddr_storage);
	event_data = static_cast<struct s_event *>(data);
	std::cout << event << std::endl;
	if (event_data->fd == event_data->socket->get_socketfd())
	{
		if ((new_socket = accept(event_data->fd, (struct sockaddr *)&client_addr, &sin_size)) < 0)
		{
			syslog(LOG_WARNING, "failed to accept new connection %m");
			return ;
		}
		set_nonblocking(new_socket);
		new_data = new struct s_event;
		new_data->fd = new_socket;
		new_data->socket = event_data->socket;
#if defined __linux__
		if (poll_add(poll, new_socket, EPOLLIN |EPOLLHUP | EPOLLRDHUP | EPOLLET, new_data) != 0)
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
	else if (event & EPOLLIN)
#endif
#if defined __APPLE__
	else if (event & EV_READ)
#endif
	{
		syslog(LOG_INFO, "new buffer from socket %d", event_data->fd);
		event_data->socket->read_socket(event_data->fd);
	}
#if defined __linux__
	else if (event & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))  //TODO: correct this and delete buffer to read_socket
#endif
#if defined __APPLE__
	else if (event & (EV_EOF | EV_ERROR))
#endif
	{
		syslog(LOG_INFO, "Connexion is close for the socket %d", event_data->fd);
		poll_del(poll, event_data->fd);
		close(event_data->fd);
		delete event_data;
	}
}
