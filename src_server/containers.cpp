#include "containers.hpp"

Containers::Containers(void)
{
	syslog(LOG_DEBUG, "[INFO]\tNew Containers create");
}

void	Containers::read_config(char *path)
{
	int	file_fd;

	if (access(path, R_OK) != 0)
		throw std::runtime_error("not read for the current file");
	file_fd = open(path, O_RDONLY);
	syslog(LOG_DEBUG, "open file %s", path);
	if (file_fd < 0)
		throw std::runtime_error(strerror(errno));
	mmap
}

void	Containers::listen(void)
{

}

void	Containers::add_server(pair_str listen, class Server *server)
{

}

Containers::~Containers()
{

}
