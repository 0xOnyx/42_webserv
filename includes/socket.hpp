#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "includes.h"
# define MAX_QUEUE 10
# define BUFFER_SIZE 4096

class Socket
{
public:
	Socket(const std::string&, const std::string& port);
	void	read_socket(int socket);
	void	add_server(const std::string& servername, class Server *server);
	void	init();
	int 	get_socketfd();
	void	delete_buff(int socket);
	~Socket();
private:
	std::string								_hostname;
	std::string								_port;
	int 									_socket_fd;
	std::map<std::string, class Server *>	_server;
	std::map<int, std::vector<char> >		_buffer;
	
};

#endif