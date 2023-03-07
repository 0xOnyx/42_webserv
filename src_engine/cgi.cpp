#include "cgi.hpp"

Cgi::Cgi(std::map<std::string, std::string> location) : Engine(location)
{

}

std::pair<std::string, std::string> Cgi::get_name(std::string &path)
{
	std::size_t	pos;
	std::string name;
	std::string path_info;

	if ((pos = path.find(_location["CGI"])) == std::string::npos)
		return (std::make_pair("", ""));
	name = path.substr(0, pos + _location["CGI"].length());
	path_info = path.substr(pos + _location["CGI"].length());
	syslog(LOG_DEBUG, "new substr for the name => %s", name.c_str());
	if ((pos = name.rfind("/")) == std::string::npos)
		return (std::make_pair("", ""));
	name = name.substr(pos);
	syslog(LOG_DEBUG, "new substr for the name => %s", name.c_str());
	return (std::make_pair(name, path_info));
}

std::string Cgi::exec_cgi(Request &request, std::string &path)
{
	char 					tmp_file_in[] = "/tmp/webservINXXXXXX";
	char					tmp_file_out[] = "/tmp/webservOUTXXXXXX";
	char 					client_str_addr[INET6_ADDRSTRLEN];
	char 					server_str_addr[INET6_ADDRSTRLEN];
	std::ostringstream		port_str_server;
	struct sockaddr_storage	client_addr = {};
	struct sockaddr_storage server_addr = {};
	socklen_t 				sin_size;
	int 					fd_body = -1;
	int 					fd_out = -1;
	std::vector<char>		buff_res;
	int 					pid;
	int 					status_pid;
	struct stat				stat_out = {};
	std::string 			res_http;
	std::pair<std::string, std::string>	name_to_exec;

	sin_size = sizeof(struct sockaddr_storage);
	name_to_exec = get_name(path);
	name_to_exec.first = _location["exec"] + name_to_exec.first;
	syslog(LOG_DEBUG, "name of exec => %s", name_to_exec.first.c_str());
	syslog(LOG_DEBUG, "name of uri info => %s", name_to_exec.second.c_str());

//If the GET method is used, the data comes from the QUERY_STRING variable.
//If the POST method is used, this information is sent to your program using standard input.
	if ((fd_body = mkstemp(tmp_file_in)) < 0
		|| (fd_out = mkstemp(tmp_file_out)) < 0)
	{
		fd_body > 0 ? close(fd_body) : 0;
		fd_out > 0 ? close(fd_out): 0;
		syslog(LOG_ERR, "failed to create tmp file for the cgi %m");
		throw std::runtime_error("failed to create tmp file for the cgi");
	}
	if (unlink(tmp_file_in) < 0 || unlink(tmp_file_out) < 0)
	{
		close(fd_body);
		close(fd_out);
		syslog(LOG_ERR, "failed unlink tmp file for the cgi %m");
		throw std::runtime_error("failed unlink tmp file for the cgi");
	}
	unsigned int len = request.get_body().length();
	if (std::atoi(_location["max_body"].c_str()) > 0)
		len = std::atoi(_location["max_body"].c_str());
	if (write(fd_body, request.get_body().c_str(), len) < 0
	|| lseek(fd_body, 0, SEEK_SET))
	{
		close(fd_body);
		close(fd_out);
		syslog(LOG_ERR, "failed to write body to tmp file for the cgi %m");
		throw std::runtime_error("failed to write body to tmp file for the cgi");
	}
	if ((pid = fork()) == 0)
	{
		if (dup2(fd_body, STDIN_FILENO) < 0
			|| dup2(fd_out, STDOUT_FILENO) < 0)
		{
			close(fd_body);
			close(fd_out);
			exit(1);
		}
		close(fd_body);
		close(fd_out);
		if (getpeername(request.socketfd, (struct sockaddr *)&client_addr, &sin_size) < 0
			|| getsockname(request.socketfd, (struct sockaddr *)&client_addr, &sin_size) < 0)
		{
			close(fd_out);
			close(fd_body);
			syslog(LOG_ERR, "failed to get peername or getsockname %d %m", request.socketfd);
			exit(2);
		}
		if (client_addr.ss_family == AF_INET)
			inet_ntop(client_addr.ss_family, (const void *)&((struct sockaddr_in *)&client_addr)->sin_addr, client_str_addr, INET_ADDRSTRLEN);
		else if (client_addr.ss_family == AF_INET6)
			inet_ntop(client_addr.ss_family, (const void *)&((struct  sockaddr_in6 *)&client_addr)->sin6_addr, client_str_addr, INET6_ADDRSTRLEN);

		if (server_addr.ss_family == AF_INET)
		{
			inet_ntop(server_addr.ss_family, (const void *)&((struct sockaddr_in *)&server_addr)->sin_addr, server_str_addr, INET_ADDRSTRLEN);
			port_str_server << ntohs(((struct sockaddr_in *)&client_addr)->sin_port);
		}
		else if (server_addr.ss_family == AF_INET6)
		{
			inet_ntop(server_addr.ss_family, (const void *)&((struct  sockaddr_in6 *)&server_addr)->sin6_addr, server_str_addr, INET6_ADDRSTRLEN);
			port_str_server << ntohs(((struct sockaddr_in6 *)&client_addr)->sin6_port);
		}
		if (
#if defined __linux__
				clearenv() != 0 ||
#endif
			   setenv("REDIRECT_STATUS", "200", 1) != 0
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
			|| setenv("PATH_INFO", name_to_exec.second.c_str(), 1) != 0
			|| setenv("PATH_TRANSLATED", path.c_str(), 1) != 0
			|| setenv("DOCUMENT_ROOT", _location["root"].c_str(), 1) != 0
			|| setenv("QUERY_STRING", request.getURIComp(QUERY).length() > 0 ? request.getURIComp(QUERY).c_str()  + 1 : "", 1) != 0
			|| setenv("REMOTE_ADDR", client_str_addr, 1) != 0
			|| setenv("REMOTE_HOST", client_str_addr, 1) != 0
			|| setenv("REMOTE_IDENT", request.getHeaderValue("Authorization").c_str(), 1) != 0
			|| setenv("REMOTE_USER", request.getHeaderValue("Authorization").c_str(), 1) != 0
			|| setenv("REQUEST_URI", request.getURI().c_str(), 1) != 0
			|| setenv("REQUEST_METHOD", request.request_line[METHOD].c_str(), 1) != 0
			|| setenv("SCRIPT_NAME", name_to_exec.first.c_str(), 1) != 0
			|| setenv("SCRIPT_FILENAME", _location["exec"].c_str(), 1) != 0
			|| setenv("SERVER_NAME", server_str_addr, 1) != 0
			|| setenv("SERVER_PORT", port_str_server.str().c_str(), 1) != 0
			|| setenv("SERVER_PROTOCOL", request.request_line[PROTOCOL].c_str(), 1) != 0
			|| setenv("SERVER_SOFTWARE", NAME, 1) != 0)
		{
			close(fd_body);
			close(fd_out);
			exit(3);
		}
		if (execl(name_to_exec.first.c_str(), _location["exec"].c_str(), NULL) != 0)
		{
			syslog(LOG_ERR, "failed to fork the cgi %m");
			exit(4);
		}
	}
	else if (pid < 0)
	{
		syslog(LOG_ERR, "failed to fork cgi %m");
		throw std::runtime_error("failed to fork cgi");
	}
	syslog(LOG_DEBUG, "WAIT FOR CGI %d", pid);
	if (waitpid(pid, &status_pid, 0) < 0
		|| (WIFEXITED(status_pid) && WEXITSTATUS(status_pid) != 0))
	{
		close(fd_body);
		close(fd_out);
		syslog(LOG_DEBUG, "failed to wait pid %d error -> %d %m", pid, WEXITSTATUS(status_pid));
		throw std::runtime_error("failed to wait pid ");
	}
	if (lseek(fd_out, 0, SEEK_SET) < 0
		|| fstat(fd_out, &stat_out) < 0)
	{
		close(fd_body);
		close(fd_out);
		syslog(LOG_DEBUG, "failed get info from file %m");
		throw std::runtime_error("failed get info from file");
	}
	buff_res.resize((ssize_t)stat_out.st_size);
	if (read(fd_out, buff_res.data() ,(size_t)stat_out.st_size) < 0)
	{
		close(fd_body);
		close(fd_out);
		syslog(LOG_DEBUG, "failed to read from file out size %lu %m ", (size_t)stat_out.st_size);
		throw std::runtime_error("failed to read from file out size");
	}
	close(fd_out);
	close(fd_body);
	res_http = std::string(buff_res.begin(), buff_res.end());
	syslog(LOG_DEBUG, "resultat from cgi is => \n%s", res_http.c_str());
	return (res_http);
}

std::string Cgi::process_request(Request &request)
{
	std::vector<char>	file_content;
	std::string 		file_str;
	std::string 		path;

	if (_location["methods"].find(request.request_line[METHOD]) == std::string::npos)
	{
		//create not authorized Responsse
		syslog(LOG_DEBUG, "error methods not allow");
		return (generate_error(405, _location));
	}
	path = _location["root"];
	if (_location["path"] == request.getURIComp(PATH))
		path += "/" + _location["index"];
	else
		path += request.getURIComp(PATH);
	if (path.find(_location["CGI"]) != std::string::npos)
	{
		try{
			std::string 	cgi_str;

			cgi_str =  this->exec_cgi(request, path);
			CGIParser	cgiparser(cgi_str);
			const std::string &body = cgiparser.getBody();
			//const std::map<std::string, std::string>	&header = cgiparser.getHeaders(); TODO : MODIFY THIS BY NEW VERSION OF RESPONSE CLASS

			return (Response(200, body.size(), mimes::get_type(".html"), body).getResponse());
		}
		catch(std::exception &e)
		{
			syslog(LOG_ERR, "runtime error => %s", e.what());
			return (generate_error(500, _location));
		}
	}
	else
	{
		syslog(LOG_DEBUG, "process by static_server");
		Static_serv static_serv(_location);
		return (static_serv.process_request(request));
	}
}

Cgi::~Cgi()
{

}
