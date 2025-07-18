/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:44:03 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/03 15:44:06 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	new_word_token(t_lexer **lexer, char **cmd)
{
	int		quote;
	int		idx;
	char	*new_content_token;

	quote = 0;
	idx = 0;
	while ((*cmd)[idx])
	{
		if (!quote && ft_strchr(" \t\n<>|", (*cmd)[idx]))
			break ;
		if (!quote && ((*cmd)[idx] == '\'' || (*cmd)[idx] == '"'))
			quote = (*cmd)[idx];
		else if (quote && (*cmd)[idx] == quote)
			quote = 0;
		idx++;
	}
	if (quote)
		return (fprintf(stderr, "Syntax error: unclosed quote\n"),
			free_lexer(*lexer, 0), 0);
	new_content_token = ft_substr(*cmd, 0, idx);
	add_token(lexer, new_token(new_content_token, WORD));
	*cmd += idx;
	return (1);
}

t_lexer	*token_recognition(char *cmd)
{
	t_lexer	*lexer;

	lexer = NULL;
	if (!cmd)
		return (NULL);
	while (cmd && *cmd)
	{
		while (ft_strchr(" \t\n", *cmd) && *cmd)
			cmd++;
		if (!*cmd)
			break ;
		if (ft_strchr("<>|", *cmd))
			new_ope_token(&lexer, &cmd);
		else if (!new_word_token(&lexer, &cmd))
		{
			return (NULL);
		}
	}
	return (lexer);
}

t_lexer	*start_lexer(char *cmd_line)
{
	t_lexer	*lexer;

	lexer = token_recognition(cmd_line);
	if (!lexer)
		return (NULL);
	return (lexer);
}
