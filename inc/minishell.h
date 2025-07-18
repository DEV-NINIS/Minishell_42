/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:45:35 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/01 16:45:37 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SUCCESS_FREE 1
# define FAILED_FREE 0

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_token
{
	WORD,
	PIPE,
	D_LESS,
	D_GREAT,
	LESS,
	GREAT
}						t_token;

typedef struct s_char_list
{
	char				*content;
	struct s_char_list	*next;
	struct s_char_list	*previous;

}						t_char_list;

typedef struct s_lexer
{
	char				*content;
	t_token				token;
	struct s_lexer		*next;
	struct s_lexer		*prev;
}						t_lexer;

enum					e_type_signal
{
	NO_RESULT = -1,
	CRTL_C = 0,
	CRTL_D = 4,
	CRTL_BACKSLASH = 28,
};

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT
}						t_node_type;

// parsing structure
typedef struct s_cmd_node
{
	char				**args;
}						t_cmd_node;

typedef struct s_pipe_node
{
	struct s_ast		*left;
	struct s_ast		*right;
}						t_pipe_node;

typedef struct s_redirect_node
{
	struct s_ast		*command;
	char				*filename;
	t_token				redirect_type;
}						t_redirect_node;

typedef struct s_ast
{
	t_node_type			type;
	union
	{
		t_cmd_node		cmd;
		t_pipe_node		pipe;
		t_redirect_node	redirect;
	};
}						t_ast;

typedef struct s_expand_values
{
	int					i;
	int					in_single_quote;
	int					in_double_quote;
	char				*result;
	char				tmp[2];
	char				*joined;
	char				*expanded;
	char				*tmp2;
}						t_expand_values;

typedef struct s_env
{
	char				*current_key;
	char				*current_value;
	struct s_env		*next;
	struct s_env		*previous;
}						t_env;

typedef struct s_cmd
{
	char				**args;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	int					heredoc_fd;
	char				*heredoc_delimiter;
	struct s_cmd		*next;
	struct s_cmd		*previous;
}						t_cmd;

typedef struct s_sig
{
	int					sigint;
	int					sigquit;
	int					exit_status;
	pid_t				pid;
}						t_sig;

// init env
int						ft_strlen_key(char *str_complete_line);
int						ft_strlen_value(char *str_complete_line);
void					*init_env_list_cpy_txt(t_env **current, char **env,
							int count);
t_env					*init_env_list(char **env);
t_env					*init_default_env(void);
t_env					**add_env(t_env **env, char *key, char *value);
void					free_env(t_env **env);

// signals
void					handle_signal_interrupt(int sig);

// parsing
t_lexer					*handle_line(char *line);
char					*cpy_word(char *line, int *count);

char					*get_pwd(void);
// built in
void					builtin_pwd(void);
void					builtin_env(t_env **env);
int						is_echo_n_option(const char *arg);
t_env					**builtin_export(t_env **env, t_cmd **args);
void					builtin_unset(t_env **env, char **args);
int						builtin_echo(char **args);
int						builtin_unset_test_equal(t_env **env,
							char *value_delete, int count_forward, t_env *temp);
int						modifies_env(t_cmd *cmd);
int						builtin_cd(char **args, t_env **env);
void					builtin_exit(t_cmd *cmd);

// exec
int						if_is_builtin(t_cmd *cmd);
t_env					**execute_builtin(t_cmd *cmd, t_env **envp);

char					*add_path_build(char *word_command, char *path_content);
char					**convert_l_env_to_char_env(t_env **env);
int						convert_l_env_to_char_env_utils(t_env **env,
							char **result, int *count);

void					execve_with_path_resolution(t_cmd *cmd, t_env **env);
int						execute_simple_command(t_cmd *cmd, t_env **envp);
int						execute_pipeline(t_cmd *cmds, t_env **env);
void					read_line_heredoc(t_char_list **list, char *arg_end);
char					*get_my_line_here_doc(void);
void					launch_child_pipeline(t_cmd *cmd, t_env **envp);
void					make_heredoc_in_out_file(t_cmd *cmd, t_sig *sig);
int						create_heredoc_pipe(const char *delimiter);
void					make_enter_file_exec_pipeline(t_cmd *cmd, int *prev_fd);
void					make_out_file_exec_pipeline(t_cmd *cmd, int *fd_0,
							int *fd_1);
void					make_heredoc_in_out_file_utils(t_cmd *cmd, t_sig *sig);
int						make_exec_builtins_simple_command(t_cmd *cmd,
							t_env **envp, t_sig *sig);
void					child_process(t_cmd *cmd, t_env **env, int prev_fd,
							int fd[2]);
void					parent_process(int *prev_fd, int fd[2], t_cmd *cmd);
void					wait_all(int *exit_status);
void					make_exec_pipeline(t_cmd *cmd, t_env **env);

void					*push_back_list(char *content, t_char_list **list);
char					**from_chain_list_to_2star_char(t_char_list **list);
void					free_char_list(t_char_list **list);

char					*expand_string(const char *input, t_env **env,
							int exit_status);
void					expand_cmd_args(t_cmd *cmd, t_env **env,
							int exit_status);
void					expand_all_commands(t_cmd *cmds, t_env **env,
							int exit_status);
char					*expand_variable(const char *str, int *i, t_env **env,
							int exit_status);

int						expand_string_in_while(const char *input, t_env **env,
							int exit_status, t_expand_values *expand_values);
void					expand_lexer(t_lexer *lexer, t_env **env,
							int exit_status);

int						expand_handle_dollar(const char *input, t_env **env,
							int exit_status, t_expand_values *v);
void					expand_handle_regular_char(const char *input,
							t_expand_values *v);

void					free_expand_values(t_expand_values *expand_values);

// gestion des variables d'environnement
char					*get_env_value(char *key, t_env **env);
int						set_env_value(t_env **env, const char *key,
							const char *value);
void					reset_env(t_env **env);

// error handling
void					print_errno_error(char *prefix, char *name);
void					print_custom_error(char *msg);
void					print_command_not_found(char *cmd);
void					print_syntax_error(char *unexpected_token);
void					handle_fork_error(void);
void					handle_pipe_error(void);
void					handle_execve_error(t_cmd *cmd);
void					handle_open_error(char *file, int is_input);
// parsing
t_lexer					*handle_line(char *line);
/* lexer */
void					new_ope_token(t_lexer **lexer, char **cmd);
int						new_word_token(t_lexer **lexer, char **cmd);
t_lexer					*token_recognition(char *cmd);
t_lexer					*start_lexer(char *cmd_line);
t_lexer					*new_token(char *content, t_token type);
t_lexer					*lex_last(t_lexer *lst);
void					add_token(t_lexer **lst, t_lexer *new);
void					print_lex(t_lexer *lexer);
t_cmd					*ast_to_cmds(t_ast *node);
void					print_ast_command(t_ast *ast);

void					advance(t_lexer **lexer);
char					**parse_args(t_lexer **lexer);
t_ast					*parse_pipeline(t_lexer **lexer);
t_ast					*parse_redirects(t_lexer **lexer);
t_ast					*handle_redirect(t_lexer **lexer, t_ast *cmd);
t_ast					*parse(t_lexer **tokens);
t_ast					*create_pipe_node(t_ast *left, t_ast *right);
void					print_ast(t_ast *ast, int depth);
void					free_string_array(char **arr);
void					free_ast(t_ast *ast);
t_ast					*create_redirection_node(t_lexer **lexer, t_ast *cmd,
							t_token redir_type);
t_ast					*build_pipeline_nodes(t_lexer **lexer, t_ast *left);
int						check_word_quotes(t_lexer *lexer, char *cmd);

// convert lexer into command
t_cmd					*lexer_to_cmds(t_lexer *lexer);
void					*free_lexer(t_lexer *lexer, int if_heredoc);
void					*free_cmd(t_cmd *cmd);

int						is_input_special(char *rl);

int						get_exit_code(int status);

extern int				g_exit_status;

#endif
