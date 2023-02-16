#include "static_serv.hpp"

Static_serv::Static_serv(std::map <std::string, std::string> location) : Engine(location)
{

}

void	Static_serv::Request(struct s_list *list)
{
	(void)list;
}

Static_serv::~Static_serv()
{

}