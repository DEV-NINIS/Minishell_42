/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:21:05 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/02 20:21:07 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	has_final_heredoc(t_cmd *cmds)
{
	t_cmd	*last;

	last = cmds;
	while (last && last->next)
		last = last->next;
	return (last && last->heredoc);
}

t_env	**execute_builtin(t_cmd *cmd, t_env **envp, t_sig *sig)
{
	int	fd;

	if (cmd->outfile)
		fd = sig->sigquit;
	else
		fd = 1;
	if (!ft_strncmp(cmd->args[0], "export", ft_strlen(cmd->args[0])))
		(builtin_export(envp, &cmd));
	else if (!ft_strncmp(cmd->args[0], "env", ft_strlen(cmd->args[0])))
		(builtin_env(envp));
	else if (!ft_strncmp(cmd->args[0], "pwd", ft_strlen(cmd->args[0])))
		builtin_pwd();
	else if (!ft_strncmp(cmd->args[0], "unset", ft_strlen(cmd->args[0])))
		builtin_unset(envp, cmd->args);
	else if (!ft_strncmp(cmd->args[0], "echo", ft_strlen(cmd->args[0])))
		(builtin_echo(cmd->args, fd));
	else if (!ft_strncmp(cmd->args[0], "cd", ft_strlen(cmd->args[0])))
		(builtin_cd(cmd->args, envp));
	else
		printf("zsh command not found\n");
	return (envp);
}

int	convert_l_env_to_char_env_utils(t_env **env, char **result, int *count)
{
	int	size_y;
	int	count2;

	result[*count] = ft_calloc(sizeof(char), ft_strlen((*env)->current_key)
			+ ft_strlen((*env)->current_value) + 2);
	if (!result[*count])
		return (-1);
	size_y = -1;
	count2 = -1;
	while ((*env)->current_key[++(size_y)])
		result[*count][size_y] = (*env)->current_key[size_y];
	result[*count][size_y] = '=';
	while ((*env)->current_value[++(count2)] && ++size_y > 0)
		result[*count][(size_y)] = (*env)->current_value[(count2)];
	(*env) = (*env)->next;
	(*count)++;
	return (1);
}

char	**convert_l_env_to_char_env(t_env **env)
{
	char	**result;
	int		size_y;
	int		count;

	size_y = 0;
	count = 0;
	while ((*env)->previous != NULL)
		(*env) = (*env)->previous;
	while ((*env)->next != NULL)
	{
		(*env) = (*env)->next;
		size_y++;
	}
	while ((*env)->previous != NULL)
		(*env) = (*env)->previous;
	result = malloc(sizeof(char *) * (size_y + 1));
	if (!result)
		return (NULL);
	result[size_y] = NULL;
	while ((*env)->next != NULL)
		convert_l_env_to_char_env_utils(env, result, &count);
	while ((*env)->previous != NULL)
		(*env) = (*env)->previous;
	return (result);
}

void	read_line_heredoc(t_char_list **list, char *arg_end)
{
	char	*rl;

	rl = get_my_line_here_doc();
	while (rl && !ft_strncmp(rl, arg_end, ft_strlen(rl)))
	{
		push_back_list(rl, list);
		free(rl);
		rl = get_my_line_here_doc();
	}
	if (rl)
		free(rl);
}
