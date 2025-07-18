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

char	*add_path_build(char *word_command, char *path_content)
{
	char	*temp;
	int		count;
	int		count2;

	count = -1;
	count2 = -1;
	temp = ft_calloc(sizeof(char), ft_strlen(word_command) + 13);
	if (!temp)
		return (NULL);
	while (path_content[++count])
		temp[count] = path_content[count];
	while (word_command[++count2])
	{
		temp[count] = word_command[count2];
		count++;
	}
	return (temp);
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

void	free_char_list(t_char_list **list)
{
	while ((**list).previous != NULL)
		(*list) = (**list).previous;
	while ((**list).next != NULL)
		free((**list).content);
}
