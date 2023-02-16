#ifndef ENGINE_HPP
# define ENGINE_HPP
# include "includes.h"

struct s_list
{

};

class Engine
{
public:
	Engine(std::map<std::string, std::string> location);
	virtual	void Request(struct s_list *list) = 0;
	virtual ~Engine();
protected:
	std::map<std::string, std::string>	_location;
};

#endif