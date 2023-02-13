#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "includes.h"
# define MAX_QUEUE 10

class Socket
{
public:
	Socket(const char *hostname, const char *port);
	void init();
	~Socket();
private:
	std::string								_hostname;
	std::string								_port;
	int 									_socket_fd;
	std::map<std::string, class *Server>	_server;
	
};

#endif