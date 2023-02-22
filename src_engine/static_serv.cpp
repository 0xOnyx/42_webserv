#include "static_serv.hpp"

Static_serv::Static_serv(std::map <std::string, std::string> location) : Engine(location)
{

}

std::string	Static_serv::process_request(Request &request)
{
	struct stat			stat_element = {};
	std::vector<char>	file_content;
	std::string 		file_str;
	std::ostringstream	res_http;
	std::string 		path;
	int 				fd = -1;
	size_t 				size_file;

	if (_location["methods"].find(request.request_line[METHOD]))
	{
		//create not authorized Responsse
		syslog(LOG_DEBUG, "error methods not allow");
		return (std::string(""));
	}
	path = _location["root"];
	if (_location["path"] == request.getURIComp(PATH))
		path += "/" + _location["index"];
	else
		path += request.getURIComp(PATH);
	syslog(LOG_DEBUG, "search file for the value => %s", path.c_str());
	if (stat(path.c_str(), &stat_element) != 0)
	{
		//create element not found
		syslog(LOG_DEBUG, "failed to stat element");
	}
	if ((stat_element.st_mode & S_IFMT) != S_IFREG)
	{
		//create not authorized Responsse
		syslog(LOG_DEBUG, "error methods not allow");
		return (std::string(""));
	}
	if (access(path.c_str(), R_OK) != 0 || (fd = open(path.c_str(), O_RDONLY)) < 0 || fstat(fd, &stat_element) != 0)
	{
		//create not authorized responsse
		if (fd >= 0)
			close(fd);
		syslog(LOG_DEBUG, "error to open file");
		return (std::string(""));
	}
	size_file = (ssize_t)stat_element.st_size;
	syslog(LOG_DEBUG, "size of the file %ld", size_file);
	file_content.resize(size_file);
	if (read(fd, file_content.data(), size_file) < 0)
	{
		close(fd);
		syslog(LOG_DEBUG, "error to read from file descriptor");
		return (std::string(""));
	}
	file_str = std::string(file_content.begin(), file_content.end());
	//TODO: MODIFY TO REAL REQUEST
	syslog(LOG_DEBUG, "content of the file %s", file_str.c_str());
	res_http << "HTTP/1.1 200 OK\r\n";
	res_http << "Content-Type: text/html\r\n";
	res_http << "Content-Length: " << size_file << "\r\n";
	res_http << "\r\n";
	res_http << file_str;
	return (res_http.str());
}

Static_serv::~Static_serv()
{

}