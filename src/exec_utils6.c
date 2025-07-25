/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 20:38:29 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/25 20:38:31 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	create_heredoc_pipe(const char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

void	make_heredoc_in_out_file_utils(t_cmd *cmd, t_sig *sig)
{
	int	flags;

	if (cmd->outfile)
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->append)
			flags = flags | O_APPEND;
		else
			flags = flags | O_TRUNC;
		sig->sigquit = open(cmd->outfile, flags, 0644);
		if (sig->sigquit == -1)
		{
			printf("ERROR: FILE\n");
			exit(EXIT_FAILURE);
		}
		dup2(sig->sigquit, STDOUT_FILENO);
		close(sig->sigquit);
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

int	handle_exit_status(void)
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
	return (g_exit_status);
}
