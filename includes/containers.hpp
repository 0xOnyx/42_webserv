#ifndef CONTAINERS_HPP
# define CONTAINERS_HPP
# include "includes.h"

class Socket;
class Server;

class Containers
{
public:
	Containers(void);
	void	read_config(char *path_file);
	void	listen(void);
	~Containers();
private:
	void	add_server(pair_str listen, class Server *server);
	std::map<pair_str, class *Socket>	_binding;
};

#endifx
