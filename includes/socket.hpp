#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "includes.h"
# define MAX_QUEUE 10

class Socket
{
public:
	Socket(const std::string&, const std::string& port);
	void	add_server(const std::string& servername, class Server *server);
	void	init();
	~Socket();
private:
	std::string								_hostname;
	std::string								_port;
	int 									_socket_fd;
	std::map<std::string, class Server *>	_server;
	
};

#endif