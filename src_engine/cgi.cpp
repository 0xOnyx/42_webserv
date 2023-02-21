#include "cgi.hpp"

Cgi::Cgi(std::map<std::string, std::string> location) : Engine(location)
{

}

std::string Cgi::request(struct s_list *list)
{
	(void)list;
	return (std::string());
}

Cgi::~Cgi()
{

}
