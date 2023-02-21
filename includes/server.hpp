#ifndef SERVER_HPP
# define SERVER_HPP
# include "includes.h"

class Server
{
public:
	Server();
	void				set_error_page(std::string &error);
	void				add_engine(std::string location, class Engine *engine);
	std::vector<char>	parse_request(Request &request, std::vector<char> &rest_buff);
	~Server();
private:
	std::string 							_error_page;
	std::map<std::string, class Engine *>	_location;
};

#endif