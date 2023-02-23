#ifndef CGI_HPP
# define CGI_HPP
# include "includes.h"

class Cgi: public Engine
{
public:
	Cgi(std::map<std::string, std::string> location);
	virtual std::string	process_request(Request &request);
	virtual ~Cgi();
private:
	std::string exec_cgi(std::string &path);
};

#endif