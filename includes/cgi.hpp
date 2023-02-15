#ifndef CGI_HPP
# define CGI_HPP
# include "includes.h"

class Cgi: public Engine
{
public:
	Cgi(std::map<std::string, std::string> location);
	virtual void	Request(t_list *list);
	virtual ~Cgi();
private:
};

#endif