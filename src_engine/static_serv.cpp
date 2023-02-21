#include "static_serv.hpp"

Static_serv::Static_serv(std::map <std::string, std::string> location) : Engine(location)
{

}

std::string	Static_serv::process_request(Request &request)
{
	struct stat		statbuf = {};
	std::string 	res_http;
	std::string 	path;
	int 			fd;
	size_t 			size_file;

	(void)request;

	//TODO: test to modify
	std::string path_file;
	path_file = "/";

	if (_location["methods"] != "GET")
	{
		//create not authorized Responsse
		syslog(LOG_INFO, "error methods not allow");
		return (std::string(""));
	}
	//TODO : ADD FILE DEFAULT index.html
	path = _location["root"];
	path += path_file;
	if (access(path.c_str(), R_OK) != -1 || (fd = open(path.c_str(), O_RDONLY)) < 0 || fstat(fd, &statbuf) != 0)
	{
		//create not authorized responsse
		close(fd);
		syslog(LOG_INFO, "error to open file");
		return (std::string(""));
	}
	size_file = (ssize_t)statbuf.st_size;


	return (res_http);
}

Static_serv::~Static_serv()
{

}