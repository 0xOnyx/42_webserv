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
	virtual	void request(std::string header) = 0;
	virtual ~Engine();
protected:
	std::map<std::string, std::string>	_location;
};

#endif