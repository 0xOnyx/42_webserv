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
PATH_UTILS			= src_utils/
PATH_OBJ			= objs/

HEADER				= 	data.h includes.h utils.h cgi.hpp config.hpp containers.hpp engine.hpp server.pp socket.hpp static.hpp
SRC_CONFIG			= 	config.cpp
SRC_ENGINE			= 	cgi.cpp engine.cpp static.cpp
SRC_ROUTINE			= 	main.cpp
SRC_SERVER			= 	containers.cpp server.cpp socket.cpp
SRC_UTIL			= 	utils.cpp

SRC_CONFIGS			= $(addprefix $(PATH_CONFIG),$(SRC_CONFIG))
SRC_ENGINES			= $(addprefix $(PATH_ENGINE),$(SRC_ENGINE))
SRC_ROUTINES		= $(addprefix $(PATH_ROUTINE),$(SRC_ROUTINE))
SRC_SERVERS			= $(addprefix $(PATH_SERVER),$(SRC_SERVER))
SRC_UTILS			= $(addprefix $(PATH_UTIL),$(SRC_UTIL))

SRCS 				= $(SRC_CONFIGS) $(SRC_ENGINES) $(SRC_ROUTINES) $(SRC_SERVERS) $(SRC_UTILS)

OBJ					= $(SRCS:.c=.o)
OBJS				= $(addprefix $(PATH_OBJ),$(OBJ))
HEADERS				= $(addprefix $(PATH_HEADER),$(HEADER))

ifndef DEBUG
	DEBUG			= 0
endif

DEBUGING			= -g3 -fsanitize=address
CXXFLAGS				= -Wall -Wextra -Werror


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

$(PATH_OBJ)$(PATH_PARSER)%.o	: $(PATH_PARSER)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_PARSER)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_CYAN)DEBUG => [%s] $(COLOR_BOLD)PARSER\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_ROUTINE)%.o	: $(PATH_ROUTINE)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_ROUTINE)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_BLUE)DEBUG => [%s] $(COLOR_BOLD)ROUTINE\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_RAYTRACING)%.o: $(PATH_RAYTRACING)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_RAYTRACING)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)RAYTRACING\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_GARBAGE)%.o	: $(PATH_GARBAGE)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_GARBAGE)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_YELLOW)DEBUG => [%s] $(COLOR_BOLD)GARBAGE\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_UTIL)%.o		: $(PATH_UTIL)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_UTIL)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)UTIL\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_MLX_UTILS)%.o		: $(PATH_MLX_UTILS)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_MLX_UTILS)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_CYAN)DEBUG => [%s] $(COLOR_BOLD)MLX UTIL\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<


$(MLX):
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_CYAN)MLX$(COLOR_RESET)\n"
	@make -C $(PATH_MLX)

$(NAME)		: $(MLX) $(OBJS)
	@$(CC) $(CXXFLAGS) $(OPTIONS) -o $(@) $(OBJS) $(LIBS)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] LINKAGE $(COLOR_BOLD)ALL OBJS FILE =>\n\t $(COLOR_WHITE)$(^:.o=.o\n\t)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION FINISH !$(COLOR_WHITE)$(COLOR_RESET_BOLD)"

clean		:
	@$(RM) $(OBJS)
	@$(RM) $(PATH_OBJ)
	@make -C $(PATH_MLX) clean
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] DELETE $(COLOR_BOLD)ALL OBJS FILE =>\n\t $(COLOR_WHITE)$(OBJS:.o=.o\n\t)"
	@make -C $(PATH_MLX) clean
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] CLEAN MLX !$(COLOR_RESET)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] CLEAN FINISH !$(COLOR_RESET)"

fclean		: clean
	@$(RM) $(NAME)
	@$(RM) $(PATH_MLX)/libmlx.a
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] DELETE $(COLOR_BOLD)PROGRAMME =>\n\t $(COLOR_WHITE)$(NAME)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] FCLEAN FINISH !$(COLOR_RESET)"

re			: fclean all

.PHONY: all fclean clean re $(MLX)