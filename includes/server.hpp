#ifndef SERVER_HPP
# define SERVER_HPP
# include "includes.h"

class Server
{
public:
	Server();
	void	set_error_page(std::string &error);
	void	add_engine(std::string location, class Engine *engine);
	~Server();
private:
	std::string 							_error_page;
	std::map<std::string, class Engine *>	_location;
};

#endif