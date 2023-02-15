include .env
export
NAME = webserv

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
PATH_CONFIG			= src_config/
PATH_ENGINE			= src_engine/
PATH_ROUTINE		= src_routine/
PATH_SERVER			= src_server/
PATH_UTIL			= src_utils/
PATH_OBJ			= objs/

HEADER				= cgi.hpp config.hpp containers.hpp data.h engine.hpp includes.h server.hpp socket.hpp static_serv.hpp utils.h syslog.hpp
SRC_CONFIG			= config.cpp
SRC_ENGINE			= cgi.cpp engine.cpp static_serv.cpp
SRC_ROUTINE			= main.cpp
SRC_SERVER			= containers.cpp server.cpp socket.cpp
SRC_UTIL			= utils.cpp epoll.cpp kqueue.cpp socket.cpp compare.cpp syslog.cpp handler_poll.cpp

SRC_CONFIGS			= $(addprefix $(PATH_CONFIG),$(SRC_CONFIG))
SRC_ENGINES			= $(addprefix $(PATH_ENGINE),$(SRC_ENGINE))
SRC_ROUTINES		= $(addprefix $(PATH_ROUTINE),$(SRC_ROUTINE))
SRC_SERVERS			= $(addprefix $(PATH_SERVER),$(SRC_SERVER))
SRC_UTILS			= $(addprefix $(PATH_UTIL),$(SRC_UTIL))

SRCS 				= $(SRC_CONFIGS) $(SRC_ENGINES) $(SRC_ROUTINES) $(SRC_SERVERS) $(SRC_UTILS)

OBJ					= $(SRCS:.cpp=.o)
OBJS				= $(addprefix $(PATH_OBJ),$(OBJ))
HEADERS				= $(addprefix $(PATH_HEADER),$(HEADER))

ifndef DEBUG
	DEBUG			= 0
endif

DEBUGING			= -g3 -fsanitize=address
CXXFLAGS			= -Wall -Wextra -Werror -std=c++98


OPTIONS				= -I$(PATH_HEADER)
LIBS				=
CC					= g++
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
endif

ifdef OPTIMISATION_LEVEL
	ifeq ($(OPTIMISATION_LEVEL),0)
		# Nothing
	else
		CXXFLAGS += -O$(OPTIMISATION_LEVEL)
	endif
else
	CXXFLAGS += -Ofast
endif

all			: $(NAME)

$(PATH_OBJ)$(PATH_CONFIG)%.o	: $(PATH_CONFIG)%.cpp $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_CONFIG)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_CYAN)DEBUG => [%s] $(COLOR_BOLD)CONFIG\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_ENGINE)%.o	: $(PATH_ENGINE)%.cpp $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_ENGINE)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_BLUE)DEBUG => [%s] $(COLOR_BOLD)ENGINE\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_ROUTINE)%.o	: $(PATH_ROUTINE)%.cpp $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_ROUTINE)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)ROUTINE\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_SERVER)%.o	: $(PATH_SERVER)%.cpp $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_SERVER)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_YELLOW)DEBUG => [%s] $(COLOR_BOLD)SERVER\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_UTIL)%.o		: $(PATH_UTIL)%.cpp $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_UTIL)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)UTIL\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<


$(NAME)		: $(OBJS)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) $(OBJS) $(LIBS)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] LINKAGE $(COLOR_BOLD)ALL OBJS FILE =>\n\t $(COLOR_WHITE)$(^:.o=.o\n\t)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION FINISH !$(COLOR_WHITE)$(COLOR_RESET_BOLD)"

clean		:
	@$(RM) $(OBJS)
	@$(RM) $(PATH_OBJ)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] DELETE $(COLOR_BOLD)ALL OBJS FILE =>\n\t $(COLOR_WHITE)$(OBJS:.o=.o\n\t)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] CLEAN FINISH !$(COLOR_RESET)"

fclean		: clean
	@$(RM) $(NAME)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] DELETE $(COLOR_BOLD)PROGRAMME =>\n\t $(COLOR_WHITE)$(NAME)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] FCLEAN FINISH !$(COLOR_RESET)"

re			: fclean all

.PHONY: all fclean clean re