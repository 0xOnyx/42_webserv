#include "redirect.hpp"

Redirect::Redirect(std::map<std::string, std::string> location) :Engine(location)
{

}

std::string Redirect::process_request(Request &request)
{
	std::string &link = _location["link"];
	std::string	res_http;
	(void)request;

	res_http = Response(301, _location["link"]).getResponse();
	syslog(LOG_DEBUG, "redirect to the link => %s", link.c_str());
	syslog(LOG_DEBUG, "responsse redirect link => %s", res_http.c_str());
	return (res_http);
}

Redirect::~Redirect()
{

}