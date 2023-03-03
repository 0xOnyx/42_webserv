#ifndef CONTAINERS_HPP
# define CONTAINERS_HPP
# include "includes.h"

class Socket;
class Server;

struct	ServerConfig
{
	pair_str	listen;
	std::string	servername;
	std::string	error_4xx;
	std::string error_5xx;
};

class Containers
{
public:
	Containers();
	void	read_config(char *path_file);
	void	init_socket(void);
	void	listen(void);
	~Containers();
	struct poll_t	_poll;
private:
	void	_add_location(class Server *current_server, std::map<std::string, std::string>	&location);
	void	_add_server(ServerConfig &config, class Server *server);
	void	_parse_config(char *file);
	typedef std::map<pair_str, class Socket *>	_binding_type;
	std::vector<struct s_event *>	_events;
	_binding_type	_binding;
};

#endif
