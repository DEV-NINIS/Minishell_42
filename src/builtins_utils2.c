/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:37:23 by ael-fari          #+#    #+#             */
/*   Updated: 2025/06/23 19:37:39 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	modifies_env(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (!ft_strncmp(cmd->args[0], "export", ft_strlen(cmd->args[0])))
		return (1);
	if (!ft_strncmp(cmd->args[0], "unset", ft_strlen(cmd->args[0])))
		return (1);
	if (!ft_strncmp(cmd->args[0], "cd", ft_strlen(cmd->args[0])))
		return (1);
	if (!ft_strncmp(cmd->args[0], "exit", ft_strlen(cmd->args[0])))
		return (1);
	return (0);
}

int	if_is_builtin(t_cmd *cmd)
{
	if (!ft_strncmp(cmd->args[0], "export", ft_strlen(cmd->args[0])))
		return (1);
	if (!ft_strncmp(cmd->args[0], "env", ft_strlen(cmd->args[0])))
		return (1);
	if (!ft_strncmp(cmd->args[0], "pwd", ft_strlen(cmd->args[0])))
		return (1);
	if (!ft_strncmp(cmd->args[0], "unset", ft_strlen(cmd->args[0])))
		return (1);
	if (!ft_strncmp(cmd->args[0], "cd", ft_strlen(cmd->args[0])))
		return (1);
	if (!ft_strncmp(cmd->args[0], "echo", ft_strlen(cmd->args[0])))
		return (1);
	if (!ft_strncmp(cmd->args[0], "exit", ft_strlen(cmd->args[0])))
		return (1);
	return (0);
}

int	is_echo_n_option(const char *arg)
{
	int	i;

	i = 1;
	if (!arg || arg[0] != '-')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}
