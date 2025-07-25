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
		execute_builtin(cmds, env, sig);
		return (1);
	}
	return (0);
}

int	check_if_command_valid(t_cmd *cmd, t_env **env, t_sig *sig)
{
	char	*abs_path;
	char	**path;
	char	**envp;

	if (access(cmd->args[0], X_OK) == 0)
		return (1);
	if (if_is_builtin(cmd) && ft_strncmp(cmd->args[0], "echo",
			ft_strlen(cmd->args[0])))
		return (execute_builtin(cmd, env, sig), 0);
	envp = convert_l_env_to_char_env(env);
	path = get_path(envp);
	abs_path = get_abs_path(cmd->args[0], path);
	if (!abs_path && !if_is_builtin(cmd))
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

void	child_before_pipe(t_cmd **cmd, t_env **env, int prev_fd, int fd[2])
{
	setup_signals_child();
	child_process(*cmd, env, prev_fd, fd);
}

void	parent_process_and_cmd_next(t_cmd **cmd, int *prev_fd, int fd[2])
{
	parent_process(prev_fd, fd, *cmd);
	(*cmd) = (*cmd)->next;
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
		if (!check_if_command_valid(cmd, env, &sig))
		{
			parent_process_and_cmd_next(&cmd, &prev_fd, fd);
			continue ;
		}
		sig.pid = fork();
		if (sig.pid == -1)
			return (perror("fork"), 0);
		if (sig.pid == 0)
			child_before_pipe(&cmd, env, prev_fd, fd);
		parent_process_and_cmd_next(&cmd, &prev_fd, fd);
	}
	return (handle_exit_status());
}

// int	execute_pipeline(t_cmd *cmds, t_env **env)
// {
// 	t_cmd	*cmd;
// 	t_sig	sig;
// 	int		fd[2];
// 	int		prev_fd;

// 	cmd = cmds;
// 	prev_fd = -1;
// 	make_scan_and_single_builtin_execute_pipeline(cmds, env, &sig);
// 	while (cmd)
// 	{
// 		if (cmd->next && pipe(fd) == -1)
// 			return (perror("pipe"), 0);
// 		if (!check_if_command_valid(cmd, env, &sig))
// 		{
// 			parent_process(&prev_fd, fd, cmd);
// 			cmd = cmd->next;
// 			continue ;
// 		}
// 		sig.pid = fork();
// 		if (sig.pid == -1)
// 			return (perror("fork"), 0);
// 		if (sig.pid == 0)
// 		{
// 			setup_signals_child();
// 			child_process(cmd, env, prev_fd, fd);
// 		}
// 		parent_process(&prev_fd, fd, cmd);
// 		cmd = cmd->next;
// 	}
// 	handle_exit_status();
// 	return (g_exit_status);
// }
