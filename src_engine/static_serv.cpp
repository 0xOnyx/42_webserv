#include "static_serv.hpp"

Static_serv::Static_serv(std::map <std::string, std::string> location) : Engine(location)
{

}

std::string	Static_serv::request(struct s_list *list)
{
	(void)list;
	return (std::string(""));
}

Static_serv::~Static_serv()
{

}