#ifndef ENGINE_HPP
# define ENGINE_HPP
# include "includes.h"

class Engine
{
public:
	Engine(std::map<std::string, std::string> location);
	virtual	void Request(t_list *list) = 0;
	virtual ~Engine();
protected:
	std::map<std::string, std::string>	_location;
};

#endif