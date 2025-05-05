#ifndef MINISHELL_H
# define MINISHELL_H

#define SUCCESS_FREE 1
#define FAILED_FREE 0

#include "../libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>

typedef enum Token {
    WORD,           // vaut 0
    PIPE,           // vaut 1
    REDIRECT_IN,    // vaut 2
    REDIRECT_OUT,   // vaut 3
    APPEND,         // vaut 4
    HEREDOC         // vaut 5
} t_Token;

enum type_signal
{
	NO_RESULT = -1,
	CRTL_C = 0,
	CRTL_D = 4,
	CRTL_BACKSLASH = 28,
};

typedef struct s_lexer
{
	char 			*content;
	t_Token	 		token;
	struct s_lexer	*next;
	struct s_lexer	*previous;

} t_lexer;

typedef struct s_env
{
	char			*current_key;
	char			*current_value;
	struct s_env	*next;
	struct s_env	*previous;
} t_env;

typedef struct	s_sig
{
	int				sigint;
	int				sigquit;
	int				exit_status;
	pid_t			pid;
}				t_sig;

// init env
int		ft_strlen_key(char *str_complete_line);
int		ft_strlen_value(char *str_complete_line);
void	*init_env_list_cpy_txt(t_env **current, char **env, int count);
t_env	*init_env_list(char **env);
int		add_env(t_env **env, char *key, char *value);
void	free_env(t_env **env);

// signals
void	handle_signal_interrupt(int sig);

// parsing
t_lexer	*handle_line(char *line);
char	*cpy_word(char *line, int *count);

// built in
void    builtin_pwd();
void	builtin_env(t_env **env);
void	builtin_unset(t_env **env, char *value_delete);
int		builtin_unset_test_equal(t_env **env, char *value_delete, int count_forward, t_env *temp);

// exec 
int		if_is_builtin();
void	launch_child_process(char *path, char **args, char **envp, t_sig *sig);


# endif
