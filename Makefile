NAME = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g

SRC_PATH = ./src/
OBJ_PATH = ./obj/
SRC = readline.c \
      read_line_utils.c \
      read_line_utils2.c \
      signals.c \
      builtins.c \
      env.c \
      expand.c \
      token_word.c \
      parse_line.c \
      parse_line_utils.c \
      parse_line_utils2.c \
      exec.c \
      error_handling.c \
      builtins_utils.c \
      builtins_utils2.c \
      builtins_utils3.c \
      env_utils.c \
      error_handling_utils.c \
      exec_utils.c \
      exec_utils2.c \
      exec_utils3.c \
      exec_utils4.c \
      exec_utils5.c \
      exec_utils6.c \
      expand_utils.c \
      expand_utils2.c \
      lexer.c \
      lexer_utils.c

SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJS = $(addprefix $(OBJ_PATH), $(SRC:.c=.o))

INCS = -I ./inc/ -I ./libft/
LIBDIR = ./libft/
LIBFT = $(LIBDIR)libft.a

RM = rm -rf

.PHONY: all clean fclean re bonus

all: $(LIBFT) $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT) -lreadline

$(LIBFT):
	$(MAKE) -C $(LIBDIR)

clean:
	$(RM) $(OBJ_PATH)
	$(MAKE) -C $(LIBDIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBDIR) fclean

re: fclean all
