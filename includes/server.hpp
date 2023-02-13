#ifndef SERVER_HPP
# define SERVER_HPP
# include "includes.h"

class Engine;

class Server
{
public:
	Server();
	void		add_engine(std::string location, class *Engine);
	~Server();
private:
	std::string 							error_page;
	std::map<std::string, class *Engine>	location;
};

#endif