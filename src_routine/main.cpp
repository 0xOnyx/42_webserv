#include "includes.h"

volatile bool is_running = true;

void	handler_sign(int sign)
{
	is_running = false;
	syslog(LOG_INFO, "[STOP]\tstop signal receive!");
}

void	init_sign(void)
{
	struct sigaction	action = {};

	action.sa_handler = &handler_sign;
	if (sigaction(SIGTERM, &action, NULL) != 0 || sigaction(SIGQUIT, &action, NULL))
		throw std::runtime_error("failed to set handler for signal");
}

int	main(int argc, char **argv)
{
	class Syslog		log;
	std::ostringstream	error;
	class Containers	containers;

	try{
		init_sign();
		if (argc != 2)
			throw std::runtime_error("error argument not match");
		if (*argv[1])
			containers.read_config(argv[1]);
		else
			containers.read_config(const_cast<char *>(static_cast<const char *>(DEFAULT_CONFIG)));
		containers.init_socket();
		containers.listen();
	}
	catch(std::runtime_error const &e)
	{
		error << "[RUNTIME ERROR]\t" << e.what() << std::endl;
		syslog(LOG_ERR, "%s\terrno => %m", error.str().c_str());
		return (EXIT_FAILURE);
	}
	catch(std::exception const &e)
	{
		error << "[EXECPTION]\t" << e.what() << std::endl;
		syslog(LOG_ERR, "%s\terrno => %m", error.str().c_str());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}