#ifndef ENGINE_HPP
# define ENGINE_HPP
# include "data.h"

struct s_list
{

};

class Engine
{
public:
	Engine(std::map<std::string, std::string> location);
	virtual	std::string request(std::string header);
	virtual ~Engine();
protected:
	std::map<std::string, std::string>	_location;
};

#endif