/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:06:24 by ael-fari          #+#    #+#             */
/*   Updated: 2025/06/24 20:06:40 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	make_scan_and_single_builtin_execute_pipeline(t_cmd *cmds,
		t_env **env, t_sig *sig)
{
	t_cmd	*scan;

	scan = cmds;
	while (scan)
	{
		if (scan->heredoc)
		{
			scan->heredoc_fd = create_heredoc_pipe(scan->heredoc_delimiter);
			if (scan->heredoc_fd == -1)
			{
				perror("heredoc");
				return (1);
			}
		}
		scan = scan->next;
	}
	if (!cmds->next && if_is_builtin(cmds))
	{
		make_heredoc_in_out_file(cmds, sig);
		execute_builtin(cmds, env);
		return (1);
	}
	return (0);
}

void	make_enter_file_exec_pipeline(t_cmd *cmd, int *prev_fd)
{
	int	in_fd;

	if (cmd->heredoc)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	else if (cmd->infile)
	{
		in_fd = open(cmd->infile, O_RDONLY);
		if (in_fd == -1)
		{
			perror(cmd->infile);
			exit(1);
		}
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	else if (*prev_fd != -1)
	{
		dup2(*prev_fd, STDIN_FILENO);
		close(*prev_fd);
	}
}

void	handle_exit_status(void)
{
	int	status;
	int	sig_num;

	while (wait(&status) > 0)
	{
		if (WIFSIGNALED(status))
		{
			sig_num = WTERMSIG(status);
			if (sig_num == SIGQUIT)
				write(1, "Quit (core dumped)\n", 20);
			else if (sig_num == SIGINT)
				write(1, "\n", 1);
			g_exit_status = 128 + sig_num;
		}
		else if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}

int	check_if_command_valid(t_cmd *cmd, t_env **env)
{
	char	*abs_path;
	char	**path;
	char	**envp;

	envp = convert_l_env_to_char_env(env);
	path = get_path(envp);
	abs_path = get_abs_path(cmd->args[0], path);
	if (!abs_path)
	{
		free_string_array(envp);
		free(abs_path);
		print_command_not_found(cmd->args[0]);
		return (0);
	}
	free_string_array(envp);
	free(abs_path);
	return (1);
}

int	execute_pipeline(t_cmd *cmds, t_env **env)
{
	t_cmd	*cmd;
	t_sig	sig;
	int		fd[2];
	int		prev_fd;

	cmd = cmds;
	prev_fd = -1;
	make_scan_and_single_builtin_execute_pipeline(cmds, env, &sig);
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			return (perror("pipe"), 0);
		if (!check_if_command_valid(cmd, env))
		{
			parent_process(&prev_fd, fd, cmd);
			cmd = cmd->next;
			continue ;
		}
		sig.pid = fork();
		if (sig.pid == -1)
			return (perror("fork"), 0);
		if (sig.pid == 0)
		{
			setup_signals_child();
			child_process(cmd, env, prev_fd, fd);
		}
		parent_process(&prev_fd, fd, cmd);
		cmd = cmd->next;
	}
	handle_exit_status();
	return (g_exit_status);
}
