/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:05:24 by ael-fari          #+#    #+#             */
/*   Updated: 2025/05/05 16:06:13 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	create_heredoc_pipe(const char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

void	make_heredoc_in_out_file_utils(t_cmd *cmd, t_sig *sig)
{
	int	flags;

	if (cmd->outfile)
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->append)
			flags = flags | O_APPEND;
		else
			flags = flags | O_TRUNC;
		sig->sigquit = open(cmd->outfile, flags, 0644);
		if (sig->sigquit == -1)
		{
			printf("ERROR: FILE\n");
			exit(EXIT_FAILURE);
		}
		dup2(sig->sigquit, STDOUT_FILENO);
		close(sig->sigquit);
	}
}

int	make_exec_builtins_simple_command(t_cmd *cmd, t_env **envp,
		t_sig *sig)
{
	if (if_is_builtin(cmd))
	{
		if (modifies_env(cmd))
		{
			make_heredoc_in_out_file(cmd, sig);
			execute_builtin(cmd, envp);
		}
		else
		{
			sig->pid = fork();
			if (sig->pid == 0)
			{
				make_heredoc_in_out_file(cmd, sig);
				execute_builtin(cmd, envp);
				exit(SUCCESS_FREE);
			}
			waitpid(sig->pid, &sig->exit_status, 0);
		}
		return (1);
	}
	return (0);
}

static void	exec_child_process(t_cmd *cmd, t_env **envp, t_sig *sig)
{
	char	**en;
	char	*full_path;

	make_heredoc_in_out_file(cmd, sig);
	en = convert_l_env_to_char_env(envp);
	if (cmd->args[0][0] == '.' || cmd->args[0][0] == '/')
	{
		if (access(cmd->args[0], X_OK) == 0)
			execve(cmd->args[0], cmd->args, en);
		else
		{
			perror("minishell");
			exit(126);
		}
	}
	full_path = add_path_build(cmd->args[0], "/usr/bin/");
	if (access(full_path, X_OK) == 0)
		execve(full_path, cmd->args, en);
	free(full_path);
	handle_execve_error(cmd);
	exit(EXIT_FAILURE);
}

int	execute_simple_command(t_cmd *cmd, t_env **envp)
{
	t_sig	sig;

	if (make_exec_builtins_simple_command(cmd, envp, &sig))
	{
		g_exit_status = 0;
		return (g_exit_status);
	}
	sig.pid = fork();
	if (sig.pid == -1)
		return (0);
	if (sig.pid == 0)
		exec_child_process(cmd, envp, &sig);
	else
		waitpid(sig.pid, &sig.exit_status, 0);
	g_exit_status = get_exit_code(sig.exit_status);
	return (g_exit_status);
}
