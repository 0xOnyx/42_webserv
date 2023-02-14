#ifndef STATIC_HPP
# define STATIC_HPP
# include "includes.h"

class Static_serv: public Engine
{
public:
	Static_serv(std::map<std::string, std::string> location);
	void	Request(t_list *list);
	~Cgi();
private:

};

#endif