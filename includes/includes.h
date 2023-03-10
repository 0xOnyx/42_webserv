#ifndef INCLUDES_H
# define INCLUDES_H
# define NAME "WEBSERVER/1.0"
# define DEFAULT_CONFIG "./conf/default.conf"
# define DEFAULT_ERROR "./www/errorPages.html"

extern volatile bool is_running;

# include "data.h"
# include "utils.h"
# include "request.hpp"
# include "response.hpp"
# include "engine.hpp"
# include "containers.hpp"
# include "server.hpp"
# include "socket.hpp"
# include "syslog.hpp"
# include "cgi.hpp"
# include "cgi_parser.hpp"
# include "redirect.hpp"
# include "static_serv.hpp"
# include "mimes.hpp"
# include "response.hpp"

#endif