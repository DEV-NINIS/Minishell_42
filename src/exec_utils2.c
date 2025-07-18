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

void	make_exec_pipeline(t_cmd *cmd, t_env **env)
{
	char	**envp;
	char	*full_path;

	if (if_is_builtin(cmd))
	{
		execute_builtin(cmd, env);
		exit(0);
	}
	else
	{
		envp = convert_l_env_to_char_env(env);
		full_path = add_path_build(cmd->args[0], "/usr/bin/");
		execve(full_path, cmd->args, envp);
		print_command_not_found(cmd->args[0]);
		exit(127);
	}
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

void	make_out_file_exec_pipeline(t_cmd *cmd, int *fd_0, int *fd_1)
{
	int	out_fd;
	int	flags;

	if (cmd->outfile)
	{
		if (cmd->append)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		out_fd = open(cmd->outfile, flags, 0644);
		if (out_fd == -1)
		{
			perror(cmd->outfile);
			exit(127);
		}
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	else if (cmd->next)
	{
		close(*fd_0);
		dup2(*fd_1, STDOUT_FILENO);
		close(*fd_1);
	}
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
		sig.pid = fork();
		if (sig.pid == -1)
			return (perror("fork"), 0);
		if (sig.pid == 0)
			child_process(cmd, env, prev_fd, fd);
		parent_process(&prev_fd, fd, cmd);
		cmd = cmd->next;
	}
	wait_all(&sig.exit_status);
	return (g_exit_status);
}
