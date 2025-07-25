/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:13:32 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/03 17:13:33 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int			g_exit_status;

static int	execute_if_valid(t_cmd *cmds, t_env **env, t_lexer *lexer,
		int *if_p)
{
	if (!cmds)
	{
		printf("error command\n");
		return (0);
	}
	if (cmds->next)
	{
		g_exit_status = execute_pipeline(cmds, env);
		if (*if_p)
			free_lexer(lexer, 0);
		else
			free_lexer(lexer, 0);
		*if_p = 0;
	}
	else
	{
		g_exit_status = execute_simple_command(cmds, env, lexer);
		free_lexer(lexer, 0);
	}
	return (1);
}

int	check_exit(t_cmd *cmds)
{
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (!ft_strcmp(cmd->args[0], "exit"))
		{
			if (builtin_exit(cmds) != -100)
				return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}

static int	handle_command_line_utils(t_cmd *cmds, char *rl, t_lexer *head,
		t_ast *head_ast)
{
	if (!cmds)
	{
		free(rl);
		free_lexer(head, 0);
		free_ast(head_ast);
		return (0);
	}
	if (check_exit(cmds))
	{
		free_cmd(cmds);
		free(rl);
		free_ast(head_ast);
		free_lexer(head, 0);
		return (1);
	}
	return (0);
}

int	handle_command_line(char *rl, t_lexer *lexer, t_env **env,
		int *if_p)
{
	t_ast	*parsed;
	t_cmd	*cmds;
	t_lexer	*head;
	t_ast	*head_ast;

	head = lexer;
	expand_lexer(lexer, env, g_exit_status);
	parsed = parse(&lexer);
	head_ast = parsed;
	cmds = ast_to_cmds(parsed);
	if (handle_command_line_utils(cmds, rl, head, head_ast))
		return (free_env(env), exit(g_exit_status), 1);
	free_ast(parsed);
	execute_if_valid(cmds, env, head, if_p);
	free_cmd(cmds);
	free(rl);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_env	*env_result;
	int		if_first_pipe_command;

	g_exit_status = 0;
	if_first_pipe_command = 1;
	if (!argc || !argv)
		return (0);
	if (env[0])
		env_result = init_env_list(env);
	else
		env_result = init_default_env();
	setup_signals_shell();
	signal(SIGINT, handle_signal_interrupt);
	signal(SIGQUIT, SIG_IGN);
	minishell_loop(&env_result, &if_first_pipe_command);
	free_env(&env_result);
	return (0);
}
