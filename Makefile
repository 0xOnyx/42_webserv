include .env
export
NAME = webserv
CGI	= cgi-bin/c.cgi
NODE = cgi-bin/node.cgi

COLOR_ESC			= \033
COLOR_WHITE			= $(COLOR_ESC)[97m
COLOR_CYAN			= $(COLOR_ESC)[96m
COLOR_GREEN			= $(COLOR_ESC)[32m
COLOR_BLUE			= $(COLOR_ESC)[34m
COLOR_YELLOW		= $(COLOR_ESC)[33m
COLOR_MAGENTA 		= $(COLOR_ESC)[35m
COLOR_BOLD			= $(COLOR_ESC)[1m
COLOR_RESET			= $(COLOR_ESC)[0m
COLOR_RESET_BOLD	= $(COLOR_ESC)[21m

PATH_HEADER			= includes/
PATH_ENGINE			= src_engine/
PATH_ROUTINE		= src_routine/
PATH_SERVER			= src_server/
PATH_UTIL			= src_utils/
PATH_CGI			= src_cgi/
PATH_OBJ			= objs/

HEADER				= cgi.hpp cgi_parser.hpp containers.hpp data.h engine.hpp includes.h mimes.hpp redirect.hpp request.hpp response.hpp server.hpp socket.hpp static_serv.hpp syslog.hpp utils.h
SRC_ENGINE			= cgi.cpp engine.cpp static_serv.cpp redirect.cpp
SRC_ROUTINE			= main.cpp
SRC_SERVER			= containers.cpp server.cpp socket.cpp
SRC_UTIL			= utils.cpp epoll.cpp kqueue.cpp socket_utils.cpp compare.cpp syslog.cpp handler_poll.cpp request.cpp cgi_parser.cpp response.cpp mimes.cpp generate_error.cpp
SRC_CGI				= cgi.c
SRC_NODE			= node.js

SRC_ENGINES			= $(addprefix $(PATH_ENGINE),$(SRC_ENGINE))
SRC_ROUTINES		= $(addprefix $(PATH_ROUTINE),$(SRC_ROUTINE))
SRC_SERVERS			= $(addprefix $(PATH_SERVER),$(SRC_SERVER))
SRC_UTILS			= $(addprefix $(PATH_UTIL),$(SRC_UTIL))
SRC_CGIS			= $(addprefix $(PATH_CGI),$(SRC_CGI))

SRCS 				= $(SRC_ENGINES) $(SRC_ROUTINES) $(SRC_SERVERS) $(SRC_UTILS)

OBJ					= $(SRCS:.cpp=.o)
CGI_O				= $(SRC_CGIS:.c=.o)
OBJS				= $(addprefix $(PATH_OBJ),$(OBJ))
CGIS_O				= $(addprefix $(PATH_OBJ),$(CGI_O))
HEADERS				= $(addprefix $(PATH_HEADER),$(HEADER))

ifndef DEBUG
	DEBUG			= 0
endif

DEBUGING			= -g3 -fsanitize=address -fsanitize=undefined
CXXFLAGS			= -Wall -Wextra -Werror -std=c++98
CFLAGS				= -Wall -Wextra -Werror

OPTIONS				= -I$(PATH_HEADER)
LIBS				=
CPP					= g++
CC					= gcc
RM					= rm -rf

UNAME = $(shell uname -s)

ifeq ($(UNAME), Linux)
	LIBS			+=
	OPTIONS			+=
endif

ifeq ($(UNAME), Darwin)
	LIBS 			+=
	OPTIONS 		+=
endif

ifeq ($(DEBUG), 1)
	CXXFLAGS += $(DEBUGING)
	CFLAGS += $(DEBUGING)
endif

ifdef OPTIMISATION_LEVEL
	ifeq ($(OPTIMISATION_LEVEL),0)
		# Nothing
	else
		CXXFLAGS += -O$(OPTIMISATION_LEVEL)
		CFLAGS += -O$(OPTIMISATION_LEVEL)
	endif
else
	CXXFLAGS += -Ofast
	CFLAGS += -Ofast
endif

all			: $(NAME)

$(PATH_OBJ)$(PATH_ENGINE)%.o	: $(PATH_ENGINE)%.cpp $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_ENGINE)
	@$(CPP) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_BLUE)DEBUG => [%s] $(COLOR_BOLD)ENGINE\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_ROUTINE)%.o	: $(PATH_ROUTINE)%.cpp $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_ROUTINE)
	@$(CPP) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)ROUTINE\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_SERVER)%.o	: $(PATH_SERVER)%.cpp $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_SERVER)
	@$(CPP) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_YELLOW)DEBUG => [%s] $(COLOR_BOLD)SERVER\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_UTIL)%.o		: $(PATH_UTIL)%.cpp $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_UTIL)
	@$(CPP) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)UTIL\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_CGI)%.o		: $(PATH_CGI)%.c
	@mkdir -p $(PATH_OBJ)$(PATH_CGI)
	@$(CC) $(CFLAGS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)CGI\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<


cgi: $(CGI)

$(CGI):  $(CGIS_O)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] LINKAGE $(COLOR_BOLD)CGI OBJS FILE =>\n\t $(COLOR_WHITE)$(CGIS_O:.o=.o\n\t)"
	@$(CC) $(CFLAGS) -o $@ $(CGIS_O)

node: $(NODE)

$(NODE):
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] INSTALL $(COLOR_BOLD)CGI NODE FILE =>\n\t $(COLOR_WHITE)$(PATH_CGI)$(SRC_NODE)\n\t"
	@npm install parse-multipart-data
	@cp $(PATH_CGI)$(SRC_NODE) $@

$(NAME)		: $(OBJS) $(CGI) $(NODE)
	@$(CPP) $(CXXFLAGS) $(OPTIONS) -o $(@) $(OBJS) $(LIBS)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] LINKAGE $(COLOR_BOLD)ALL OBJS FILE =>\n\t $(COLOR_WHITE)$(OBJS:.o=.o\n\t)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION FINISH !$(COLOR_WHITE)$(COLOR_RESET_BOLD)"

clean		:
	@$(RM) $(OBJS)
	@$(RM) $(CGIS_O)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] DELETE $(COLOR_BOLD)ALL OBJS FILE CGI =>\n\t $(COLOR_WHITE)$(CGIS_O:.o=.o\n\t)"
	@$(RM) $(PATH_OBJ)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] DELETE $(COLOR_BOLD)ALL OBJS FILE $(NAME) =>\n\t $(COLOR_WHITE)$(OBJS:.o=.o\n\t)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] CLEAN FINISH !$(COLOR_RESET)"

fclean		: clean
	@$(RM) $(NAME)
	@$(RM) $(CGI)
	@$(RM) $(NODE)
	@npm remove parse-multipart-data
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] DELETE $(COLOR_BOLD)PROGRAMME AND CGI =>\n\t $(COLOR_WHITE)$(NAME)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] FCLEAN FINISH !$(COLOR_RESET)"

re			: fclean all

.PHONY: all fclean clean re