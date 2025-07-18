/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:47:18 by ael-fari          #+#    #+#             */
/*   Updated: 2025/06/23 19:47:20 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_errno_error(char *prefix, char *name)
{
	ft_putstr_fd("Minishell: ", 2);
	if (prefix)
	{
		ft_putstr_fd(prefix, 2);
		ft_putstr_fd(": ", 2);
	}
	if (name)
	{
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(strerror(errno), 2);
}

void	print_custom_error(char *msg)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putendl_fd(msg, 2);
}

void	print_command_not_found(char *cmd)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
}
