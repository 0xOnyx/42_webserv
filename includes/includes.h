#ifndef INCLUDES_H
# define INCLUDES_H
# define NAME "WEBSERVER"
# define DEFAULT_CONFIG "./conf/default.conf"

extern volatile bool is_running;

# include "data.h"
# include "utils.h"
# include "cgi.hpp"
# include "containers.hpp"
# include "config.hpp"
# include "engine.hpp"
# include "server.hpp"
# include "socket.hpp"
# include "syslog.hpp"
# include "static_serv.hpp"

#endif