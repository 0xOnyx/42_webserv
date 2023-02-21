#ifndef ENGINE_HPP
# define ENGINE_HPP
# include "data.h"
# include "request.hpp"

class Engine
{
public:
	Engine(std::map<std::string, std::string> location);
	virtual	std::string process_request(Request &request);
	virtual ~Engine();
protected:
	std::map<std::string, std::string>	_location;
};

#endif