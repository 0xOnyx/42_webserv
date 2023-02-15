#include "containers.hpp"

Containers::Containers() :_binding(std::map<pair_str, class Socket *>()), _poll()
{
	syslog(LOG_DEBUG, "[INFO]\tNew Containers create");
	_poll.cb = handler_poll;
	if (poll_create(&_poll) != 0)
		throw std::runtime_error("Error to create poll");
}

void	Containers::read_config(char *path)
{
	char 								*buff;
	int									fd_file;
	size_t								size_file;

	if (access(path, R_OK) != 0)
		throw std::runtime_error("not read for the current file");
	fd_file = open(path, O_RDONLY);
	syslog(LOG_DEBUG, "open file %s", path);
	if (fd_file < 0)
		throw std::runtime_error(strerror(errno));
	size_file = get_size_file(fd_file);
	if ((buff = (char *)mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd_file, 0)) == MAP_FAILED)
	{
		close(fd_file);
		throw std::runtime_error("error mapping file");
	}
	this->_parse_config(buff);
	munmap((void *)buff, size_file);
	close(fd_file);
}

void	Containers::_parse_config(char *file)
{
	class Engine									*engine;
	class Server									*current_server;
	char	 										*token;
	std::string 									keyword;
	std::map<std::string, std::string>				location;
	std::map<std::string, std::string>::iterator	iterator_location;
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
			if (current_server != NULL)
			{
				_add_server(config, current_server);
				current_server = NULL;
				location = std::map<std::string, std::string>();
			}
			else
				current_server = new class Server();
		}
		else if (keyword == "listen")
		{
			ss >> keyword;
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
		}
		else if (keyword == "error")
		{
			ss >> config.error;
			if (!current_server)
				throw std::runtime_error("not set a error if server not declare");
			current_server->set_error_page(config.error);
		}
		else if (keyword == "location")
		{
			if (!location.empty())
			{
				if (!current_server)
					throw std::runtime_error("not set a location if server not declare");
				if ((iterator_location = location.find("path")) == location.end() || iterator_location->second.length() <= 0)
					throw std::runtime_error("the path is not set for the location");
				if ((iterator_location = location.find("CGI")) != location.end())
				{
					engine = dynamic_cast<Engine *>(new Cgi(location));
					current_server->add_engine(iterator_location->second, engine);
				}
				else
				{
					engine = dynamic_cast<Engine *>(new Static_serv(location));
					current_server->add_engine(iterator_location->second, engine);
				}
				location = std::map<std::string, std::string>();
			}
			ss >> location["path"];
		}
		else if (keyword == "index" || keyword == "methods" || keyword == "root" || keyword == "CGI" || keyword == "exec")
		{
			ss >> location[keyword];
		}
		token = strtok(NULL, "\n");
	}
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
			throw std::runtime_error("error during epoll process");
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
