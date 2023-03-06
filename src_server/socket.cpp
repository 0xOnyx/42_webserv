#include "socket.hpp"

Socket::Socket(const std::string& hostname, const std::string& port)
	: _hostname(hostname), _port(port), _socket_fd(-1),
	  _server(std::map<std::string, class Server *>()),
	  _read_buffer(std::map<int, std::vector<char> >()),
	  _write_buffer(std::map<int, std::vector<char> >())
{
	if (hostname.empty() && port.empty())
		throw std::runtime_error("no hostname and port set");
}

void	Socket::add_server(const std::string& servername, class Server *server)
{
	if (!_server.insert(std::pair<std::string, class Server *>(servername, server)).second)
		throw std::runtime_error("the server could not be put");
}

void	Socket::delete_buff(int socket)
{
	_read_buffer.erase(socket);
	_write_buffer.erase(socket);
}

int 	Socket::read_socket(int socket)
{
	int 									current_size;
	std::string::size_type 					res_pos;
	std::string 							current_str;
	std::vector<char>::size_type			size;
	std::vector<char>	&current_buffer 	= _read_buffer[socket];
	std::vector<char>	&current_write 		= _write_buffer[socket];

	size = current_buffer.size();
	current_buffer.resize(size + BUFFER_SIZE);
	if ((current_size = recv(
			socket,
			&current_buffer[size],
			BUFFER_SIZE,
			0)) < 0)
	{
		syslog(LOG_ERR, "Error with socket to read %d %m", socket);
		return (3);
	}
	if (current_size == 0)
	{
		return (3);
	}
	current_buffer.resize(size + current_size);
	current_str = std::string(current_buffer.begin(), current_buffer.end());
	if ((res_pos = current_str.find("\r\n\r\n")) != std::string::npos)
	{
		std::string header = current_str.substr(0, res_pos -1);
		std::string buff_rest = current_str.substr(res_pos + std::string("\r\n\r\n").size());

		current_buffer.clear();
		syslog(LOG_DEBUG, "buff rest => %s", buff_rest.c_str());
		std::copy(buff_rest.begin(), buff_rest.end(), std::back_inserter(current_buffer));
		syslog(LOG_DEBUG, "value of request header -> %s", header.c_str());
		try
		{
			Request 											request(socket, header);
			std::string											host;
			std::string::size_type								pos;
			std::map<std::string, class Server *>::iterator		iter;

			syslog(LOG_DEBUG, "New header %s", header.c_str());
			if ((host = request.getHeaderValue("Host")).empty())
				host = _server.begin()->first;
			else if (host.find(':') != std::string::npos)
			{
				pos = host.find(':');
				host = host.substr(0, pos);
			}
			syslog(LOG_DEBUG, "search server for %s", host.c_str());
			if ((iter = _server.find(host)) == _server.end())
			{
				iter = _server.begin();
				syslog(LOG_DEBUG, "no server found for %s", host.c_str());
			}
			current_write = iter->second->parse_request(request, current_buffer);
		}
		catch(std::exception const &e)
		{
			std::string res_error = Response(400).getResponse();

			syslog(LOG_DEBUG, "bad request error 400 %s", e.what());
			std::copy(res_error.begin(), res_error.end(), std::back_inserter(current_write));
		}
		return (1);
	}
	return (0);
}

int	Socket::write_socket(int socket)
{
	ssize_t 						current_size;
	ssize_t 						to_send;
	std::vector<char>				&current_write = _write_buffer[socket];
	std::vector<char>::size_type	size;

	size = current_write.size();
	to_send = size >= BUFFER_SIZE ? BUFFER_SIZE : (ssize_t)size;
	if ((current_size = send(socket, current_write.data(), to_send, 0 )) < 0)
		syslog(LOG_ERR, "Error with socket to write %d %m ", socket);
	while (current_size--)
		current_write.erase(current_write.begin());
	if (current_write.empty())
	{
		syslog(LOG_INFO, "finish to send all element from %d", socket);
		_write_buffer.erase(socket);
		_read_buffer.erase(socket);
		return (1);
	}
	return (0);
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
	{
		freeaddrinfo(res);
		throw std::runtime_error("Could not bind socket to any address");
	}
	if (listen(_socket_fd, MAX_QUEUE) == -1)
		throw std::runtime_error(strerror(errno));
	set_nonblocking(_socket_fd);
	freeaddrinfo(res);
}

Socket::~Socket()
{
	std::map<std::string, class Server *>::iterator iter;

	syslog(LOG_DEBUG, "Socket destructor called");
	for (iter = _server.begin(); iter != _server.end(); iter++)
		delete iter->second;
	close(_socket_fd);

}