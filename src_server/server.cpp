#include "server.hpp"

void	Server::set_error_page(std::string error)
{
	if (access(error.c_str(), R_OK) != 0)
		throw std::runtime_error("no acces to error page");
	_error_page = error;
}