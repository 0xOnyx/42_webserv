#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "includes.h"
# define MAX_QUEUE 1000
# define BUFFER_SIZE 4096

class Socket
{
public:
	Socket(const std::string&, const std::string& port);
	int 	read_socket(int socket);
	int 	write_socket(int socket);
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
	std::map<int, std::vector<char> >		_read_buffer;
	std::map<int, std::vector<char> >		_write_buffer;
};

#endif