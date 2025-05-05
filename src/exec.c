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

void launch_child_process(char *path, char **args, char **envp, t_sig *sig)
{
	int status;

	sig->pid = fork();
	if (sig->pid < 0)
	{
		perror("fork failed");
		sig->exit_status = 1;
		return;
	}
	if (sig->pid == 0)
	{
		execve(path, args, envp);
		perror("execve failed");
		exit(127);
	}
	else
	{
		if (waitpid(sig->pid, &status, 0) == -1)
		{
			perror("waitpid failed");
			sig->exit_status = 1;
			return;
		}
		if ((status & 0x7f) == 0)
			sig->exit_status = (status >> 8) & 0xff;
		else if ((status & 0x7f) != 0)
			sig->exit_status = 128 + (status & 0x7f);
		else
			sig->exit_status = 1;
	}
}


