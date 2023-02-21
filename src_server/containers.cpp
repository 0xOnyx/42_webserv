#include "containers.hpp"

Containers::Containers() :_poll(), _binding(std::map<pair_str, class Socket *>())
{
	syslog(LOG_DEBUG, "[INFO]\tNew Containers create");
	_poll.cb = handler_poll;
	if (poll_create(&_poll) != 0)
		throw std::runtime_error("Error to create poll");
}

void	Containers::read_config(char *path)
{
	char 								*buff;
	char 								*write_buff;
	int									fd_file;
	size_t								size_file;

	if (access(path, R_OK) != 0)
	{
		throw std::runtime_error("not read for the current file");
	}
	fd_file = open(path, O_RDONLY);
	syslog(LOG_DEBUG, "open file %s", path);
	if (fd_file < 0)
	{
		throw std::runtime_error(strerror(errno));
	}
	size_file = get_size_file(fd_file);
	if ((buff = (char *)mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd_file, 0)) == MAP_FAILED)
	{
		close(fd_file);
		throw std::runtime_error("error mapping file");
	}
	write_buff = (char *)alloca(size_file + 1);
	write_buff[size_file] = 0;
	memcpy(write_buff, buff, size_file);
	this->_parse_config(write_buff);
	munmap((void *)buff, size_file);
	close(fd_file);
}

void	Containers::_add_location(class Server *current_server, std::map<std::string, std::string>	&location)
{
	class Engine									*engine;
	std::map<std::string, std::string>::iterator	iterator_location;

	if (!current_server)
		throw std::runtime_error("not set a location if server not declare");
	if ((iterator_location = location.find("path")) == location.end() || iterator_location->second.length() <= 0)
		throw std::runtime_error("the path is not set for the location");;
	if (location.find("CGI") != location.end())
	{
		engine = dynamic_cast<class Engine *>(new Cgi(location));
		syslog(LOG_DEBUG, "add cgi engine");
		current_server->add_engine(location["path"], engine);
	}
	else
	{
		engine = dynamic_cast<class Engine *>(new Static_serv(location));
		syslog(LOG_DEBUG, "add Static server");
		current_server->add_engine(location["path"], engine);
	}
	location = std::map<std::string, std::string>();
}

void	Containers::_parse_config(char *file)
{
	class Server									*current_server;
	char	 										*token;
	std::string 									keyword;
	std::map<std::string, std::string>				location;
	struct ServerConfig								config;

	current_server = NULL;
	token = strtok(file, "\n");
	while (token != NULL)
	{
		std::istringstream ss((std::string(token)));

		ss >> keyword;
		if (keyword == "server")
		{
			syslog(LOG_DEBUG, "new server creation from file");
			if (current_server)
			{
				if (!location.empty())
					_add_location(current_server, location);
				_add_server(config, current_server);
			}
			location = std::map<std::string, std::string>();
			current_server = new class Server();
		}
		else if (keyword == "listen")
		{
			ss >> keyword;
			keyword.erase(keyword.find(';'), 1);
			syslog(LOG_DEBUG, "listen => %s", keyword.c_str());
			if (keyword.find(':') != std::string::npos)
			{
				config.listen.first = keyword.substr(0, keyword.find(':'));
				config.listen.second = keyword.substr(keyword.find(':') + 1);
			}
			else
			{
				config.listen.first = "";
				config.listen.second = keyword;
			}
		}
		else if (keyword == "server_name")
		{
			if (!current_server)
				throw std::runtime_error("not set a servername if server not declare");
			ss >> config.servername;
			config.servername.erase(config.servername.find(';'), 1);
			syslog(LOG_DEBUG, "servername => %s", config.servername.c_str());
		}
		else if (keyword == "error")
		{
			ss >> config.error;
			config.error.erase(config.error.find(';'), 1);
			syslog(LOG_DEBUG, "error page => %s", config.error.c_str());
			if (!current_server)
				throw std::runtime_error("not set a error if server not declare");
			current_server->set_error_page(config.error);
		}
		else if (keyword == "location")
		{
			if (!location.empty())
				_add_location(current_server, location);
			ss >> location["path"];
			syslog(LOG_DEBUG, "new location => %s", location["path"].c_str());
		}
		else if (keyword == "index" || keyword == "root" || keyword == "CGI" || keyword == "exec")
		{
			ss >> location[keyword];
			location[keyword].erase(location[keyword].find(';'), 1);
			syslog(LOG_DEBUG, "new value for location => %s : %s", keyword.c_str(), location[keyword].c_str());
		}
		else if (keyword == "methods")
		{
			std::string	out;
			while (ss >> out)
				location[keyword] += out;
			location[keyword].erase(location[keyword].find(';'), 1);
			syslog(LOG_DEBUG, "new value for location => %s : %s", keyword.c_str(), location[keyword].c_str());
		}
		token = strtok(NULL, "\n");
	}
	if (!location.empty())
		_add_location(current_server, location);
	_add_server(config, current_server);
}

void	Containers::init_socket()
{
	_binding_type::iterator	iterator;
	struct s_event			*data;

	for (iterator = _binding.begin(); iterator != _binding.end(); iterator++)
	{
		syslog(LOG_DEBUG, "INIT socket for the host:port => %s:%s", iterator->first.first.c_str(), iterator->first.second.c_str());
		iterator->second->init();
		data = new struct s_event;
		data->fd = iterator->second->get_socketfd();
		data->socket = iterator->second;
#if defined __linux__
		if (poll_add(&_poll, iterator->second->get_socketfd(), EPOLLIN | EPOLLOUT | EPOLLET, data) != 0)
			throw std::runtime_error("error to add to poll file descriptor");
#endif
#if defined __APPLE__
		if (poll_add(&_poll, iterator->second->get_socketfd(), EVFILT_READ , data) != 0)
			throw std::runtime_error("error to add to poll file descriptor");
#endif
	}
}

void	Containers::listen()
{
	while (is_running){
		if (poll_wait(&_poll, 20))
			throw std::runtime_error("event during epoll process");
	}
}

void	Containers::_add_server(ServerConfig &config, class Server *server)
{
	_binding_type::iterator	iterator;
	class Socket			*socket;

	if ((iterator = _binding.find(config.listen)) == _binding.end())
	{
		socket = new Socket(config.listen.first, config.listen.second);
		iterator = _binding.insert(std::pair<pair_str, class Socket *>(config.listen, socket)).first;
	}
	iterator->second->add_server(config.servername, server);
}

Containers::~Containers()
{
	if (close(_poll.fd) != 0)
		syslog(LOG_ERR, "failed to close poll %m");
}
