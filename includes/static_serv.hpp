#ifndef STATIC_HPP
# define STATIC_HPP
# include "includes.h"

class Static_serv: public Engine
{
public:
	Static_serv(std::map<std::string, std::string> location);
	std::string	process_request(Request &request);
	~Static_serv();
private:
	std::string directory_listen(std::string &path);
};

#endif