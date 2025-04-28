NAME 	= minishell
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra

SRC_PATH = ./src/
OBJ_PATH = obj/

SRC		= readline.c \
		  signals.c \
		  builtins.c \
		  env.c


SRCS		= $(addprefix $(SRC_PATH), $(SRC))
OBJ			= $(SRC:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))
INCS		= -I ./inc/


LIBDIR = ./libft/
LIBFT = ./libft/libft.a


RM 			:= rm -rf
MAKEFLAGS	+= --no-print-directory

all: compile_libft make_obj_dir $(NAME) 

$(OBJ_PATH)%.o: $(SRC_PATH)%.c 
		$(CC) $(CFLAGS) -I$(LIBDIR) -c $< -o $@ $(INCS) -g

make_obj_dir:
		mkdir obj -p

$(NAME): $(OBJ_PATH) $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT) -lreadline

compile_libft:
		$(MAKE) -C $(LIBDIR)

clean:
	$(RM) obj

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
re: fclean
	$(MAKE) -C $(LIBDIR) fclean
	$(MAKE) all

.PHONY: all clean fclean re bonus
