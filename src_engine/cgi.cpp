#include "cgi.hpp"

Cgi::Cgi(std::map<std::string, std::string> location) : Engine(location)
{

}

std::string Cgi::process_request(Request &request)
{
	(void)request;
	return (std::string());
}

Cgi::~Cgi()
{

}
