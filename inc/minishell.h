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

typedef struct char_list
{
	char					*content;
	struct char_list		*next;
	struct char_list		*previous;

} t_char_list;

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

typedef struct s_cmd
{
	char        	**args;     // Arguments de la commande
    char        	*infile;    // Redirection entrée (ou NULL)
    char        	*outfile;   // Redirection sortie (ou NULL)
    int         	append;     // 1 si c'est >>
    int         	heredoc;
	char			*heredoc_delimiter;
	struct s_cmd	*next;
	struct s_cmd	*previous;
} t_cmd;

typedef struct s_sig
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
char	*add_path_build(char *word_command, char *path_content);
char	**convert_l_env_to_char_env(t_env **env);
int		convert_l_env_to_char_env_utils(t_env **env, char **result, int *count);

void	execute_simple_command(t_cmd *cmd, t_env **envp);
void 	execute_pipeline(t_cmd *cmds, t_env **env);
void	read_line_heredoc(t_char_list **list, char *arg_end);
char	*get_my_line_here_doc();
void	launch_child_pipeline(t_cmd *cmd, t_env **envp);

// utils chain list
void	*push_back_list(char *content, t_char_list **list);
char	**from_chain_list_to_2star_char(t_char_list **list);
void	free_char_list(t_char_list **list);

// 
char	**convert_chain_to_star_char(t_char_list **list_chain);
int		convert_chain_to_star_char_utils(t_char_list **list_chain, char **result, int *count);

char    *expand_string(const char *input, t_env *env, int exit_status);
void    expand_cmd_args(t_cmd *cmd, t_env *env, int exit_status);
void    expand_all_commands(t_cmd *cmds, t_env *env, int exit_status);

// static t_sig g_sig;

# endif
