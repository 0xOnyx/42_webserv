#include "server.hpp"

Server::Server() :_error_page(), _location(std::map<std::string, class Engine *>())
{

}

void	Server::add_engine(std::string location, class Engine *engine)
{
	if (!_location.insert(std::make_pair(location, engine)).second)
		throw std::runtime_error("failed to add new engine to socket");
}

void	Server::set_error_page(std::string error)
{
	if (access(error.c_str(), R_OK) != 0)
		throw std::runtime_error("no access to error page");
	_error_page = error;
}

Server::~Server()
{

}