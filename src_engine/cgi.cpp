#include "cgi.hpp"

Cgi::Cgi(std::map<std::string, std::string> location) : Engine(location)
{

}

void Cgi::request(struct s_list *list)
{
	(void)list;
}

Cgi::~Cgi()
{

}
