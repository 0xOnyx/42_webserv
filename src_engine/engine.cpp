#include "engine.hpp"

Engine::Engine(std::map<std::string, std::string> location) : _location(location)
{

}

std::string	Engine::request(std::string header)
{
	(void)header;
	return (std::string(""));
}

Engine::~Engine()
{

}