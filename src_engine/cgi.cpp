#include "cgi.hpp"

Cgi::Cgi(std::map<std::string, std::string> location) : Engine(location)
{

}

std::string Cgi::exec_cgi(Request &request, std::string &path)
{
	char 				template_tmp[] = "/tmp/webservXXXXXX";
	int 				fd_body;
	std::vector<char>	buff_res;
	int 				pid;

//If the GET method is used, the data comes from the QUERY_STRING variable.
//If the POST method is used, this information is sent to your program using standard input.
	if ((pid = fork()) == 0)
	{


		if ((fd_body = mkstemp(template_tmp)) < 0)
		{
			exit(1);
		}
		if (clearenv() != 0
			|| setenv("REDIRECT_STATUS", "200", 1) != 0
			|| setenv("AUTH_TYPE", request.getHeaderValue("Authorization").c_str(), 1) != 0
			|| setenv("CONTENT_LENGTH", request.getHeaderValue("Content-Length").c_str(), 1) != 0
			|| setenv("CONTENT_TYPE", request.getHeaderValue("Content-type").c_str(), 1) != 0
			|| setenv("GATEWAY_INTERFACE", "CGI/1.1", 1) != 0
			|| setenv("HTTP_USER_AGENT",  request.getHeaderValue("User-Agent").c_str(), 1) != 0
			|| setenv("HTTP_HOST", request.getHeaderValue("Host").c_str(), 1) != 0
			|| setenv("HTTP_ACCEPT", request.getHeaderValue("Accept").c_str(), 1) != 0
			|| setenv("HTTP_ACCEPT_CHARSET", request.getHeaderValue("Accept-Charset").c_str(), 1) != 0
			|| setenv("HTTP_ACCEPT_LANGUAGE", request.getHeaderValue("Accept-Language").c_str(), 1) != 0
			|| setenv("HTTP_CONNECTION", request.getHeaderValue("Connection").c_str(), 1) != 0
			|| setenv("HTTP_REFERER", request.getHeaderValue("Referer").c_str(), 1) != 0
			|| setenv("HTTP_USER_AGENT", request.getHeaderValue("User-Agent").c_str(), 1) != 0
			|| setenv("PATH_INFO", request.getURIComp(""), 1) != 0
			|| setenv("PATH_TRANSLATED", , 1) != 0
			|| setenv("QUERY_STRING", , 1) != 0
			|| setenv("REMOTE_ADDR", , 1) != 0
			|| setenv("REMOTE_HOST", , 1) != 0
			|| setenv("REMOTE_IDENT", , 1) != 0
			|| setenv("REMOTE_USER", , 1) != 0
			|| setenv("REQUEST_URI", request.getURI().c_str(), 1) != 0
			|| setenv("REQUEST_METHOD", , 1) != 0
			|| setenv("SCRIPT_NAME", , 1) != 0
			|| setenv("SERVER_NAME", , 1) != 0
			|| setenv("SERVER_PORT", , 1) != 0
			|| setenv("SERVER_PROTOCOL", , 1) != 0
			|| setenv("SERVER_SOFTWARE", )), 1 != 0)
		{
			close(fd_body);
			return (std::string(""));
		}

		_location["exec"];


	}
	if else (pid < 0)
	{
		//TODO : create request error server
		return (std::string(""));
	}
	return (std::string(buff_res.begin(), buff_res.end()));
}

std::string Cgi::process_request(Request &request)
{
	std::vector<char>	file_content;
	std::string 		file_str;
	std::ostringstream	res_http;
	std::string 		path;

	if (_location["methods"].find(request.request_line[METHOD]) == std::string::npos)
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
	if (path.find(_location["CGI"]) != std::string::npos)
	{
		res_http << this->exec_cgi(request, path);
	}
	else
	{
		syslog(LOG_DEBUG, "process by static_server");
		Static_serv static_serv(_location);
		res_http << static_serv.process_request(request);
	}
	return (res_http.str());
}

Cgi::~Cgi()
{

}
