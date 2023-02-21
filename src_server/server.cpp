#include "server.hpp"

Server::Server() :_error_page(DEFAULT_ERROR), _location(std::map<std::string, class Engine *>())
{

}

void	Server::add_engine(std::string location, class Engine *engine)
{
	if (!_location.insert(std::make_pair(location, engine)).second)
		throw std::runtime_error("failed to add new engine to socket");
}

void	Server::set_error_page(std::string &error)
{
	if (access(error.c_str(), R_OK) != 0)
		throw std::runtime_error("no access to error page ");
	_error_page = error;
}

/* wait update to request class to uncomment
void	Server::read_body(Request &request, std::vector<char> &rest_buff)
{
	std::string 					key;
	std::vector<char>::size_type	content_len;
	std::vector<char>::size_type	size_buffer;
	int 							return_size;

	size_buffer = rest_buff.size();
	if (request.has_body() && !(key = request.getHeaderValue("Content-Length")).empty())
	{
		content_len = std::atoi(key.c_str());
		rest_buff.resize(content_len);
		if ((return_size = recv(request.socketfd,
								&rest_buff[size_buffer], content_len - size_buffer, 0)) < 0))
		{
			syslog(LOG_ERR, "ERROR to read body from socket %d %m", request.socketfd);
			throw std::runtime_error("error to read body from sockt");
		}
		request.set_body(std::string(rest_buff.begin(), rest_buff.end()));
	}
}
*/
std::vector<char>	Server::parse_request(Request &request, std::vector<char> &rest_buff)
{
	std::vector<char>								res_http;
	std::string 									res_engine;
	std::string										key;
	std::map<std::string, class Engine *>::iterator	iter;

	(void)rest_buff; //TODO : DELTE THIS LINE
///	read_body(request, rest_buff); TODO: Uncomment this line
	key = request.request_line[URI];
	if ((iter = _location.find(key)) == _location.end())
	{
		syslog(LOG_DEBUG, "not found location for the value => %s", key.c_str());
		iter = _location.begin();
	}
	syslog(LOG_DEBUG, "found location for the value => %s", iter->first.c_str());
	res_engine = iter->second->process_request(request);
	std::copy(res_engine.begin(), res_engine.end(), std::back_inserter(res_http));
	return (res_http);
}

Server::~Server()
{

}