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
PATH_PARSER			= src_config/
PATH_ROUTINE		= src_routine/
PATH_RAYTRACING		= src_/
PATH_GARBAGE		= src_garbage/
PATH_MLX_UTILS		= src_mlxutils/
PATH_UTIL			= src_util/
PATH_OBJ			= objs/

HEADER				= 	data.h includes.h parser.h routine.h raycasting.h garbage.h utils.h
SRC_PARSER			= 	parser.c valid_arg.c get_map.c file.c ft_strmap.c fill_map.c parser_utils.c
SRC_ROUTINE			= 	routine.c init_mlx.c render_frame.c key.c calc_new_position.c
SRC_RAYTRACING		= 	raytracing.c calc_projection_plane.c short_len.c short_len_utils.c texture_draw.c
SRC_GARBAGE			= 	garbage.c free.c malloc.c
SRC_UTIL			= 	get_data.c ft_itoa.c ft_mem.c ft_split.c ft_str.c ft_putstr.c \
						ft_atoi.c ft_is.c ft_readline.c math_utils.c \
						ft_strjoin.c ft_strmem.c rad.c
SRC_MLX_UTIL		=	mlx_put_rec.c mlx_utils.c mlx_print_point.c

SRC_PARSERS			= $(addprefix $(PATH_PARSER),$(SRC_PARSER))
SRC_ROUTINES		= $(addprefix $(PATH_ROUTINE),$(SRC_ROUTINE))
SRC_RAYTRACINGS		= $(addprefix $(PATH_RAYTRACING),$(SRC_RAYTRACING))
SRC_GARBAGES		= $(addprefix $(PATH_GARBAGE),$(SRC_GARBAGE))
SRC_UTILS			= $(addprefix $(PATH_UTIL),$(SRC_UTIL))
SRC_MLX_UTILS		= $(addprefix $(PATH_MLX_UTILS),$(SRC_MLX_UTIL))

SRCS 				= $(SRC_PARSERS) $(SRC_ROUTINES) $(SRC_RAYTRACINGS) $(SRC_GARBAGES) $(SRC_UTILS) $(SRC_MLX_UTILS)

OBJ					= $(SRCS:.c=.o)
OBJS				= $(addprefix $(PATH_OBJ),$(OBJ))
HEADERS				= $(addprefix $(PATH_HEADER),$(HEADER))

ifndef DEBUG
	DEBUG			= 0
endif

DEBUGING			= -g3 -fsanitize=address
CFLAGS				= -Wall -Wextra -Werror


OPTIONS				= -I$(PATH_HEADER)
LIBS				= -lm
CC					= gcc
RM					= rm -rf

UNAME = $(shell uname -s)

ifeq ($(UNAME), Linux)
	LIBS			+= -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz
	OPTIONS			+= -Imlx_linux
	PATH_MLX		= mlx_linux
	MLX				= mlx_linux
endif

ifeq ($(UNAME), Darwin)
	LIBS 			+= -Lmlx_macos -lmlx -framework OpenGL -framework AppKit
	OPTIONS 		+= -Imlx_macos
	PATH_MLX 		= mlx_macos
	MLX				= mlx_macos
endif

ifeq ($(DEBUG), 1)
	CFLAGS += $(DEBUGING)
endif

ifdef OPTIMISATION_LEVEL
	ifeq ($(OPTIMISATION_LEVEL),0)
		# Nothing
	else
		CFLAGS += -O$(OPTIMISATION_LEVEL)
	endif
else
	CFLAGS += -Ofast
endif

all			: $(NAME)

test:
	echo $(UNAME)

$(PATH_OBJ)$(PATH_PARSER)%.o	: $(PATH_PARSER)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_PARSER)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_CYAN)DEBUG => [%s] $(COLOR_BOLD)PARSER\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_ROUTINE)%.o	: $(PATH_ROUTINE)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_ROUTINE)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_BLUE)DEBUG => [%s] $(COLOR_BOLD)ROUTINE\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_RAYTRACING)%.o: $(PATH_RAYTRACING)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_RAYTRACING)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)RAYTRACING\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_GARBAGE)%.o	: $(PATH_GARBAGE)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_GARBAGE)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_YELLOW)DEBUG => [%s] $(COLOR_BOLD)GARBAGE\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_UTIL)%.o		: $(PATH_UTIL)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_UTIL)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)UTIL\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_MLX_UTILS)%.o		: $(PATH_MLX_UTILS)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_MLX_UTILS)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_CYAN)DEBUG => [%s] $(COLOR_BOLD)MLX UTIL\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<


$(MLX):
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_CYAN)MLX$(COLOR_RESET)\n"
	@make -C $(PATH_MLX)

$(NAME)		: $(MLX) $(OBJS)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) $(OBJS) $(LIBS)
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