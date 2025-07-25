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

static void	exec_child_process(t_cmd *cmd, t_env **envp, t_sig *sig)
{
	char	**en;
	char	**path;
	char	*abs_path;

	make_heredoc_in_out_file(cmd, sig);
	en = convert_l_env_to_char_env(envp);
	if (cmd->args[0][0] == '.' || cmd->args[0][0] == '/')
	{
		if (access(cmd->args[0], X_OK) == 0)
			execve(cmd->args[0], cmd->args, en);
		else
		{
			perror("minishell");
			exit(127);
		}
	}
	path = get_path(en);
	abs_path = get_abs_path(cmd->args[0], path);
	if (!abs_path)
	{
		print_command_not_found(cmd->args[0]);
		exit(127);
	}
	execve(abs_path, cmd->args, en);
}

int	exec_simple_child(t_cmd *cmd, t_env **envp, t_sig *sig)
{
	if (sig->pid == -1)
		return (0);
	if (sig->pid == 0)
	{
		setup_signals_child();
		exec_child_process(cmd, envp, sig);
	}
	return (1);
}

int	make_cheking_command(t_cmd *cmd, t_env **envp)
{
	char	**en;
	char	**path;
	char	*abs_path;

	en = convert_l_env_to_char_env(envp);
	path = get_path(en);
	abs_path = get_abs_path(cmd->args[0], path);
	if (!abs_path)
	{
		print_command_not_found(cmd->args[0]);
		free_string_array(en);
		free(abs_path);
		return (1);
	}
	free_string_array(en);
	free(abs_path);
	return (0);
}

int	execute_simple_command(t_cmd *cmd, t_env **envp)
{
	t_sig	sig;
	int		sig_num;

	if (make_exec_builtins_simple_command(cmd, envp, &sig))
		return (g_exit_status);
	if (make_cheking_command(cmd, envp))
		return (127);
	sig.pid = fork();
	if (!exec_simple_child(cmd, envp, &sig))
		return (0);
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(sig.pid, &sig.exit_status, 0);
		if (WIFSIGNALED(sig.exit_status))
		{
			sig_num = WTERMSIG(sig.exit_status);
			if (sig_num == SIGQUIT)
				write(1, "Quit (core dumped)\n", 20);
			else if (sig_num == SIGINT)
				write(1, "\n", 1);
		}
		setup_signals_parent();
	}
	return (get_exit_code(sig.exit_status));
}
