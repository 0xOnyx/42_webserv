#ifndef CONTAINERS_HPP
# define CONTAINERS_HPP
# include "includes.h"

class Socket;
class Server;

struct	ServerConfig
{
	pair_str	listen;
	std::string	servername;
	std::string	error;
};

class Containers
{
public:
	Containers();
	void	read_config(char *path_file);
	void	init_socket(void);
	void	listen(void);
	~Containers();
private:
	void	_parse_config(char *file);
	void	_add_server(ServerConfig &config, class Server *server);
	typedef std::map<pair_str, class Socket *>	_binding_type;
	_binding_type	_binding;
};

#endifx
