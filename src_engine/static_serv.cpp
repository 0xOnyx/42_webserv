#include "static_serv.hpp"

Static_serv::Static_serv(std::map <std::string, std::string> location) : Engine(location)
{

}

std::string Static_serv::directory_listen(std::string &path, std::string uri)
{
	std::ostringstream	http_out;
	DIR					*dir_path;
	struct dirent		*info_dir;

	http_out << "<html>\n"
	<< "<head>\n"
	<< "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
	<< "    <title>Directory listing for" << path <<  "</title>\n"
	<<	"</head>\n"
	<<	"<body>\n"
	<< "<h1>Directory listing for " << path <<	"</h1>\n"
	<< "<hr>";
	if ((dir_path = opendir(path.c_str())) == NULL)
	{
		syslog(LOG_DEBUG, "failed to open directory %s", path.c_str());
		http_out << "failed to open directory";
		http_out << "<hr>\n"
		<< "</body>\n"
		<< "</html>";
		return (http_out.str());
	}
	http_out << "<ul>";
	while ((info_dir = readdir(dir_path)) != NULL)
		http_out << "<li><a href=\"" << uri  << "/" << info_dir->d_name << "\">" << info_dir->d_name << "</a></li>\n";
	http_out << "</ul>";
	http_out << "<hr>\n"
			 << "</body>\n"
			 << "</html>";
	return (http_out.str());
}

std::string	Static_serv::process_request(Request &request)
{
	struct stat			stat_element = {};
	std::vector<char>	file_content;
	std::string 		file_type;
	std::string 		file_str;
	std::string 		path;
	int 				fd = -1;
	size_t 				size_file;

	if (_location["methods"].find(request.request_line[METHOD]) == std::string::npos)
	{
		syslog(LOG_DEBUG, "error methods not allow");
		return (Response(405).getResponse());
	}
	path = _location["root"];
	if (_location["path"] == request.getURIComp(PATH))
		path += "/" + _location["index"];
	else
		path += request.getURIComp(PATH);
	syslog(LOG_DEBUG, "search file for the value => %s", path.c_str());
	if (stat(path.c_str(), &stat_element) != 0)
	{
		syslog(LOG_DEBUG, "failed to stat element");
		return (Response(404).getResponse());
	}
	if ((stat_element.st_mode & S_IFMT) != S_IFREG)
	{
		syslog(LOG_DEBUG, "error methods not allow");
		if ((stat_element.st_mode & S_IFMT) == S_IFDIR && _location["autoindex"] == "on")
		{
			std::string auto_index = directory_listen(path, request.getURI());
			file_type = mimes::get_type(".html");
			syslog(LOG_DEBUG, "directory liste for this path => %s", path.c_str());
			return (Response(200, auto_index.size(), file_type, auto_index).getResponse());
		}
		else
		{
			syslog(LOG_DEBUG, "directory listen is not authorized for this path %s", path.c_str());
			return (Response(403).getResponse());
		}
	}
	if (access(path.c_str(), R_OK) != 0 || (fd = open(path.c_str(), O_RDONLY)) < 0 || fstat(fd, &stat_element) != 0)
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
	syslog(LOG_DEBUG, "content of the file %s", file_str.c_str());

	file_type = mimes::get_type(path.c_str());
	syslog(LOG_DEBUG, "type of the file %s", file_type.c_str());
	return (Response(200, file_str.size(), file_type, file_str).getResponse());
}

Static_serv::~Static_serv()
{

}