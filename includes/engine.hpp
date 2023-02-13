#ifndef ENGINE_HPP
# define ENGINE_HPP
# include "includes.h"

class Engine
{
public:
	Engine();
	virtual	void Request(t_list *list) = 0;
	~Engine();
private:
	std::string	index;
	std::string root_path;
};

#endif