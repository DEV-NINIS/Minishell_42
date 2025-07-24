/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:58:54 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/02 20:58:55 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	make_heredoc_in_out_file(t_cmd *cmd, t_sig *sig)
{
	int	heredoc_fd;

	if (cmd->heredoc)
	{
		heredoc_fd = create_heredoc_pipe(cmd->heredoc_delimiter);
		if (heredoc_fd == -1)
		{
			exit(EXIT_FAILURE);
		}
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	else if (cmd->infile)
	{
		sig->sigint = open(cmd->infile, O_RDONLY);
		if (sig->sigint == -1)
		{
			printf("ERROR : FILE\n");
			exit(EXIT_FAILURE);
		}
		dup2(sig->sigint, STDIN_FILENO);
		close(sig->sigint);
	}
	make_heredoc_in_out_file_utils(cmd, sig);
}

void	child_process(t_cmd *cmd, t_env **env, int prev_fd, int fd[2])
{
	char	*abs_path;
	char	**path;
	char	**envp;
	
	if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	make_enter_file_exec_pipeline(cmd, &prev_fd);
	make_out_file_exec_pipeline(cmd, &fd[0], &fd[1]);

	envp = convert_l_env_to_char_env(env);
	path = get_path(envp);
	abs_path = get_abs_path(cmd->args[0], path);
	execve(abs_path, cmd->args, envp);
	exit(127);
}

void	parent_process(int *prev_fd, int fd[2], t_cmd *cmd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->heredoc)
		close(cmd->heredoc_fd);
	if (cmd->next)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
	else
		*prev_fd = -1;
}

int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (status);
}

void	wait_all(int *exit_state)
{
	while (wait(exit_state) > 0)
		g_exit_status = get_exit_code(*exit_state);
}
