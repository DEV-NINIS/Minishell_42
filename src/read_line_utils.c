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
	if (ft_strcmp(rl, "exit") == 0)
	{
		free(rl);
		exit(127);
	}
	if (is_empty_line(rl))
	{
		free(rl);
		return (1);
	}
	return (0);
}
