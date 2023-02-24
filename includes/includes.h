#ifndef INCLUDES_H
# define INCLUDES_H
# define NAME "WEBSERVER/1.0"
# define DEFAULT_CONFIG "./conf/default.conf"
# define DEFAULT_ERROR "./www/errorPages.html"

extern volatile bool is_running;

# include "data.h"
# include "utils.h"
# include "request.hpp"
# include "engine.hpp"
# include "containers.hpp"
# include "config.hpp"
# include "server.hpp"
# include "socket.hpp"
# include "syslog.hpp"
# include "cgi.hpp"
# include "static_serv.hpp"

#endif