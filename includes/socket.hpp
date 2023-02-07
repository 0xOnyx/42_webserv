#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "includes.h"

class socket
{
public:
	socket(char *host, unsigned int port);
	listen();
	~socket();
private:
	std::map()
	char			*_host;
	unsigned int	_port;
};

#endif