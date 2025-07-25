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

int	exec_builtin_child(t_cmd *cmd, t_env **envp, t_sig *sig, t_lexer *lexer)
{
	make_heredoc_in_out_file(cmd, sig);
	if (if_is_builtin(cmd))
	{
		execute_builtin(cmd, envp, sig);
		free_env(envp);
		free_cmd(cmd);
		free_lexer(lexer, 0);
		return (0);
	}
	return (1);
}

static int	exec_child_process(t_cmd *cmd, t_env **envp, t_sig *sig,
		t_lexer *lexer)
{
	char	**en;
	char	**path;
	char	*abs_path;

	if (!exec_builtin_child(cmd, envp, sig, lexer))
		return (exit(0), 0);
	en = convert_l_env_to_char_env(envp);
	if (cmd->args[0][0] == '.' || cmd->args[0][0] == '/')
	{
		if (access(cmd->args[0], X_OK) == 0)
			execve(cmd->args[0], cmd->args, en);
		else
			return (perror("minishell"), exit(127), 0);
	}
	path = get_path(en);
	abs_path = get_abs_path(cmd->args[0], path);
	if (!abs_path)
		return (print_command_not_found(cmd->args[0]), exit(127), 0);
	execve(abs_path, cmd->args, en);
	return (0);
}

int	exec_simple_child(t_cmd *cmd, t_env **envp, t_sig *sig, t_lexer *lexer)
{
	if (sig->pid == -1)
		return (0);
	if (sig->pid == 0)
	{
		setup_signals_child();
		exec_child_process(cmd, envp, sig, lexer);
	}
	return (1);
}

int	make_cheking_command(t_cmd *cmd, t_env **envp)
{
	char	**en;
	char	**path;
	char	*abs_path;

	if (access(cmd->args[0], X_OK) == 0)
		return (0);
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

int	execute_simple_command(t_cmd *cmd, t_env **envp, t_lexer *lexer)
{
	t_sig	sig;
	int		sig_num;

	if (make_exec_builtins_simple_command(cmd, envp, &sig))
		return (g_exit_status);
	if (make_cheking_command(cmd, envp))
		return (127);
	sig.pid = fork();
	if (!exec_simple_child(cmd, envp, &sig, lexer))
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
