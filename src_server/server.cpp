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

void	Server::read_body(Request &request, std::vector<char> &rest_buff)
{
	int 							status;
	std::vector<char>::size_type	content_len;
	std::vector<char>::size_type	size_buffer;

	size_buffer = rest_buff.size();
	if ((content_len = request.has_body()))
	{

		rest_buff.resize(content_len);
		if (size_buffer < content_len)
		{
			syslog(LOG_DEBUG, "read body len => %lu", content_len - size_buffer);
			if ((status = recv(request.socketfd,
									&rest_buff[size_buffer], content_len - size_buffer, MSG_DONTWAIT)) < 0)
			{
				syslog(LOG_ERR, "ERROR to read body from socket %d error => %d %m",  request.socketfd, status);
				throw std::runtime_error("error to read body from socket");
			}
		}
		request.set_body(std::string(rest_buff.begin(), rest_buff.end()));
	}
}

std::vector<char>	Server::parse_request(Request &request, std::vector<char> &rest_buff)
{
	std::vector<char>								res_http;
	std::string 									res_engine;
	std::string										key;
	std::map<std::string, class Engine *>::iterator	iter;
	std::string::size_type							pos;

	read_body(request, rest_buff);
	key = request.getURIComp(PATH);
	syslog(LOG_DEBUG, "uri search %s", key.c_str());
	if ((pos = key.find('/', key.find('/') + 1)) != std::string::npos)
		key = key.substr(0, pos);
	syslog(LOG_DEBUG, "search location for the value => %s", key.c_str());
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
	std::map<std::string, class Engine *>::iterator 	iter;

	for(iter = _location.begin(); iter != _location.end(); iter++)
		delete iter->second;
}