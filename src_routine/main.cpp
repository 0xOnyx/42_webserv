#include "includes.h"

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	class Syslog		log;
	std::ostringstream	error;
	class Containers	containers;

	try{
		if (*argv[1])
			containers.read_config(argv[1]);
		else
			containers.read_config(DEFAULT_CONFIG)


	}
	catch(std::runtime_error const &e)
	{
		error << "[RUNTIME ERROR]\t" << e.what() << std::endl;
		syslog(LOG_ERR, "%s", error.str().c_str());
		return (EXIT_FAILURE);
	}
	catch(std::exception const &e)
	{
		error << "[EXECPTION]\t" << e.what() << std::endl;
		syslog(LOG_ERR, "%s", error.str().c_str());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}