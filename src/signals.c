/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:35:35 by ael-fari          #+#    #+#             */
/*   Updated: 2025/04/25 16:35:51 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_signal_interrupt(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 130;
	}
}

int	handle_other_signals(char *line)
{
	if (line[0] == CRTL_D)
		return (printf("exit\n"), CRTL_D);
	return (1);
}

char	*get_pwd(void)
{
	char	cwd[1024];
	char	*pwd;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		pwd = ft_strdup(cwd);
	else
		perror("pwd");
	return (pwd);
}
