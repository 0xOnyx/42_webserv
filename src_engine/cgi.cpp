#include "cgi.hpp"

Cgi::Cgi(std::map<std::string, std::string> location) : Engine(location)
{

}

std::string Cgi::exec_cgi(std::string &path)
{
	char 				template_tmp[] = "/tmp/webservXXXXXX";
	std::vector<char>	buff_res;
	int 				pid;


	if ((pid = fork()) == 0)
	{
		_location["exec"];



		if (clearenv() != 0
		|| setenv("CONTENT_LENGTH", "0", 1)
		|| setenv(""))
		{

			return (std::string(""));
		}
//If the GET method is used, the data comes from the QUERY_STRING variable.
//If the POST method is used, this information is sent to your program using standard input.

		setenv("AUTH_TYPE", );
		setenv("CONTENT_LENGTH", );
		setenv("CONTENT_TYPE", );
		setenv("GATEWAY_INTERFACE)", );
		setenv("HTTP_USER_AGENT", );
		setenv("HTTP_HOST", );
		setenv("HTTP_ACCEPT", );
		setenv("HTTP_ACCEPT_CHARSET", );
		setenv("HTTP_ACCEPT_LANGUAGE", );
		setenv("HTTP_CONNECTION", );
		setenv("HTTP_REFERER", );
		setenv("HTTP_USER_AGENT", );
		setenv("PATH_INFO", );
		setenv("PATH_TRANSLATED", );
		setenv("QUERY_STRING", );
		setenv("REMOTE_ADDR", );
		setenv("REMOTE_HOST", );
		setenv("REMOTE_IDENT", );
		setenv("REMOTE_USER", );
		setenv("REQUEST_METHOD", );
		setenv("SCRIPT_NAME", );
		setenv("SERVER_NAME", );
		setenv("SERVER_PORT", );
		setenv("SERVER_PROTOCOL", );
		setenv("SERVER_SOFTWARE", );


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
		res_http << this->exec_cgi(path);
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
