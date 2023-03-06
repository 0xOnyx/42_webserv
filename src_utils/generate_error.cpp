#include "includes.h"

std::string	generate_error(int error_status, std::map<std::string, std::string> &location)
{
	struct stat			stat_element = {};
	std::string 		error_page;
	std::string			file_str;
	std::string			file_type;
	std::vector<char>	file_content;
	int					fd = -1;
	size_t 				size_file;

	if ((error_status >= 400 && error_status < 500 && location["error_4xx"].empty()) || (error_status >= 500 && error_status < 600 && location["error_5xx"].empty()))
		return (Response(error_status).getResponse());
	if (error_status >= 400 && error_status < 500)
		error_page = location["error_4xx"];
	else
		error_page = location["error_5xx"];
	if (access(error_page.c_str(), R_OK) != 0 || (fd = open(error_page.c_str(), O_RDONLY)) < 0 || fstat(fd, &stat_element) != 0)
	{
		if (fd >= 0)
			close(fd);
		syslog(LOG_DEBUG, "error to open file %m");
		return (Response(401).getResponse());
	}
	size_file = (ssize_t)stat_element.st_size;
	syslog(LOG_DEBUG, "size of the file %ld", size_file);
	file_content.resize(size_file);
	if (read(fd, file_content.data(), size_file) < 0)
	{
		close(fd);
		syslog(LOG_DEBUG, "error to read from file descriptor");
		return (Response(500).getResponse());
	}
	close(fd);

	file_str = std::string(file_content.begin(), file_content.end());
	//syslog(LOG_DEBUG, "content of the file %s", file_str.c_str());

	file_type = mimes::get_type(error_page.c_str());
	syslog(LOG_DEBUG, "type of the file %s", file_type.c_str());
	return (Response(error_status, file_str.size(), file_type, file_str).getResponse());
}


