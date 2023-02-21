#include "engine.hpp"

Engine::Engine(std::map<std::string, std::string> location) : _location(location)
{

}

std::string	Engine::process_request(Request &request)
{
	(void)request;
	return (std::string(""));
}

Engine::~Engine()
{

}