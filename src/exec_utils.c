/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:36:02 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/05 16:36:03 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**get_path(char **envp)
{
	char	**path;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH", 4))
		{
			path = ft_split(envp[i] + 5, ':');
			return (path);
		}
		i++;
	}
	return (NULL);
}

char	*get_abs_path(char *cmd, char **all_path)
{
	char	*tmp;
	char	*command;
	int		i;

	i = 0;
	if (all_path == NULL || !cmd)
		return (NULL);
	if (access(cmd, F_OK) == 0)
		return (cmd);
	while (all_path[i])
	{
		tmp = ft_strjoin(all_path[i], "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, F_OK) == 0)
			return (free_string_array(all_path), (command));
		free(command);
		i++;
	}
	free_string_array(all_path);
	return (NULL);
}

char	*get_my_line_here_doc(void)
{
	char	*buf;

	buf = NULL;
	if (buf)
	{
		free(buf);
		buf = NULL;
	}
	buf = readline("heredoc > ");
	return (buf);
}

static int	simple_char_list_malloc(char *content, t_char_list **list)
{
	(*list) = malloc(sizeof(t_char_list));
	if (!(*list))
		return (0);
	(*list)->content = ft_strdup(content);
	if (!((*list)->content))
		return (0);
	(*list)->next = NULL;
	(*list)->previous = NULL;
	return (1);
}

void	*push_back_list(char *content, t_char_list **list)
{
	t_char_list	*temp;

	temp = (*list);
	if (!(*list))
	{
		if (!simple_char_list_malloc(content, list))
			return (NULL);
	}
	else
	{
		temp->next = malloc(sizeof(t_char_list));
		if (!temp->next)
			return (NULL);
		temp->next->previous = temp;
		temp = temp->next;
		temp->content = ft_strdup(content);
		if (!temp->content)
			return (NULL);
		temp->next = NULL;
	}
	return (NULL);
}
