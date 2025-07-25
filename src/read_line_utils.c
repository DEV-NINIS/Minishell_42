/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:15:53 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/03 17:15:54 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*getmyline(void)
{
	char	*buf;

	buf = NULL;
	if (buf)
	{
		free(buf);
		buf = NULL;
	}
	buf = readline("Minishell > ");
	return (buf);
}

int	is_empty_line(const char *str)
{
	while (str && *str)
	{
		if (!(*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v'
				|| *str == '\f' || *str == '\r'))
			return (0);
		str++;
	}
	return (1);
}

int	is_input_special(char *rl)
{
	if (rl[0] != '\0')
		add_history(rl);
	if (is_empty_line(rl))
	{
		free(rl);
		return (1);
	}
	return (0);
}

void	minishell_loop(t_env **env, int *if_p)
{
	char	*rl;
	t_lexer	*lexer;
	t_lexer	*head2;

	while (1)
	{
		rl = readline("minishell$ ");
		if (!rl)
			break ;
		if (is_input_special(rl))
			continue ;
		if (!handle_eof_signal(rl))
			break ;
		lexer = start_lexer(rl);
		head2 = lexer;
		if (!lexer)
		{
			free(rl);
			continue ;
		}
		if (handle_command_line(rl, head2, env, if_p))
			break ;
	}
}

int	check_if_heredoc(t_cmd *cmds)
{
	t_cmd	*tmp;

	tmp = cmds;
	while (tmp)
	{
		if (tmp->heredoc)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
