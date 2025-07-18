/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:48:03 by ael-fari          #+#    #+#             */
/*   Updated: 2025/06/06 17:48:07 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *unexpected_token)
{
	ft_putstr_fd("Minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(unexpected_token, 2);
	ft_putendl_fd("'", 2);
}

void	handle_fork_error(void)
{
	perror("Minishell: fork");
	exit(EXIT_FAILURE);
}

void	handle_pipe_error(void)
{
	perror("Minishell: pipe");
	exit(EXIT_FAILURE);
}

void	handle_execve_error(t_cmd *cmd)
{
	if (errno == ENOENT)
		fprintf(stderr, "%s: command not found\n", cmd->args[0]);
	else if (errno == EACCES)
		fprintf(stderr, "%s: permission denied\n", cmd->args[0]);
	else
		perror("execve");
	exit(127);
}

void	handle_open_error(char *file, int is_input)
{
	if (is_input)
		print_errno_error("input redirection", file);
	else
		print_errno_error("output redirection", file);
}
