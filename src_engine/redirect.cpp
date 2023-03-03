#include "redirect.hpp"

Redirect::Redirect(std::map<std::string, std::string> location) :Engine(location)
{

}

std::string Redirect::process_request(Request &request)
{
	//process redirect;
	std::string &link = _location["link"];
	(void)request;

	syslog(LOG_DEBUG, "redirect to the link => %s", link.c_str());
	return ("");
}

Redirect::~Redirect()
{

}