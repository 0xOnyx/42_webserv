#include "static_serv.hpp"

Static_serv::Static_serv(std::map <std::string, std::string> location) : Engine(location)
{

}

std::string	Static_serv::process_request(Request &request)
{
	std::string 	res_http;
	std::string 	path;

	(void)request;

	if (_location["methods"] != "GET")
	{
		//create not authorized Responsse
	}

	return (res_http);
}

Static_serv::~Static_serv()
{

}