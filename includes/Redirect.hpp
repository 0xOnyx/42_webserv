#ifndef REDIRECT_HPP
# define REDIRECT_HPP
# include "includes.h"

class Redirect : public Engine
{
public:
	Redirect(std::map<std::string, std::string> location);
	std::string process_request(Request &request);
	~Redirect();
private:

};

#endif
