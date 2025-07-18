/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:04:32 by ael-fari          #+#    #+#             */
/*   Updated: 2025/05/02 16:04:50 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	*free_lexer(t_lexer *lexer, int if_heredoc)
{
	t_lexer	*tmp;
	int		count;

	count = 0;
	while (lexer)
	{
		if (lexer->content && if_heredoc == 0)
			free(lexer->content);
		tmp = lexer->next;
		free(lexer);
		lexer = tmp;
		count++;
	}
	return (NULL);
}

void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	*free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
		{
			i = -1;
			while (cmd->args[++i])
				free(cmd->args[i]);
			free(cmd->args);
		}
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->heredoc_delimiter)
			free(cmd->heredoc_delimiter);
		free(cmd);
		cmd = tmp;
	}
	return (NULL);
}
