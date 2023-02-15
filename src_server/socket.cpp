#include "socket.hpp"

Socket::Socket(const std::string& hostname, const std::string& port) :_hostname(hostname), _port(port), _socket_fd(-1)
{
	if (hostname.length() <= 0 && port.length() <= 0)
		throw std::runtime_error("no hostname and port set")
}

void	Socket::add_server(const std::string& servername, class Server *server)
{
	if (_server.insert(std::pair<std::string, class Server *>(servername, server)).second)
		throw std::runtime_error("the server could not be put");
}

int 	Socket::get_socketfd()
{
	return (this->_socket_fd);
}

void	Socket::init()
{
	struct addrinfo		hints = {};
	struct addrinfo		*rp;
	struct addrinfo		*res;
	int 				status;
	int 				yes;

	yes = 1;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((status = getaddrinfo(
			const_cast<const char *>(_hostname.length() <= 0 ? NULL : _hostname.c_str()),
			const_cast<const char *>(_port.length() <= 0 ? NULL: _port.c_str()),
			&hints, &res)) != 0)
		throw std::runtime_error(gai_strerror(status));
	for (rp = res; rp != NULL; rp = rp->ai_next)
	{
		if ((_socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol )) == -1)
		{
			syslog(LOG_WARNING, "[ERROR] create socket %m");
			continue ;
		}
		if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			syslog(LOG_WARNING, "[ERROR] socket set options %m");
			close(_socket_fd);
			continue ;
		}
		if (bind(_socket_fd, rp->ai_addr, rp->ai_addrlen) == 0)
			break ;
		syslog(LOG_WARNING, "[ERROR] socket bind %m");
		close(_socket_fd);
	}
	if (rp == NULL)
		throw std::runtime_error("Could not bind socket to any address");
	if (listen(_socket_fd, MAX_QUEUE) == -1)
		throw std::runtime_error(strerror(errno));
	set_nonblocking(_socket_fd);
	freeaddrinfo(res);
}

Socket::~Socket()
{
	close(_socket_fd);
}