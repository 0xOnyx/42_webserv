#include "includes.h"

std::string	generate_error(int error_status, std::map<std::string, std::string> &location)
{



	if ((error_status >= 400 && error_status < 500 && location["error_4xx"].empty()) || (error_status >= 500 && error_status < 600 && location["error_5xx"].empty()))
		return (Response(error_status).getResponse());


}


