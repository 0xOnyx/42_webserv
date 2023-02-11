#ifndef SERVER_HPP
# define SERVER_HPP
# include "includes.h"

class Engine;

class Server : public Socket
{
public:
	Server(const char *hostname, const char *port);
	void parse(int fd);
	~Server();
private:
	std::map<std::string,
	std::map<std::string, class *Engine>	location;
};

#endif