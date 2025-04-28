#ifndef MINISHELL_H
# define MINISHELL_H

#define SUCCESS_FREE 1
#define FAILED_FREE 0

#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>


enum Token
{
	WORD,
	PIPE,
	WHITESPACE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	OPERATOR,
};
enum type_signal
{
	NO_RESULT = -1,
	CRTL_C = 0,
	CRTL_D = 4,
	CRTL_BACKSLASH = 28,
};

typedef struct s_lexer
{
	char *content;
	enum Token token;
} t_lexer;

typedef struct s_env
{
	char			*current_key;
	char			*current_value;
	struct s_env	*next;
	struct s_env	*previous;
} t_env;

// init env
int		ft_strlen_key(char *str_complete_line);
int		ft_strlen_value(char *str_complete_line);
void	*init_env_list_cpy_txt(t_env **current, char **env, int count);
t_env	*init_env_list(char **env);
int		add_env(t_env **env, char *key, char *value);
void	free_env(t_env **env);

// signals
void	handle_signal_interrupt(int sig);

// built in
void    builtin_pwd();

# endif
