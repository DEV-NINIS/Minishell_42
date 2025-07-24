/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 18:58:23 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/16 18:58:25 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	ft_is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_cmd *cmd)
{
	int	code;

	write(STDERR_FILENO, "exit\n", 5);
	if (cmd->args[1])
	{
		if (!ft_is_numeric(cmd->args[1]))
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
				cmd->args[1]);
			return (1);
		}
		if (cmd->args[2])
		{
			fprintf(stderr, "minishell: exit: too many arguments\n");
			g_exit_status = 1;
			return (1);
		}
		code = atoi(cmd->args[1]);
		exit(code % 256);
	}
	exit(g_exit_status);
}
