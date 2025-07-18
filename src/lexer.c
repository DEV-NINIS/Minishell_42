/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:12:51 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/03 17:12:52 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_lexer	*new_token(char *content, t_token type)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->content = content;
	lexer->token = type;
	lexer->next = NULL;
	lexer->prev = NULL;
	return (lexer);
}

t_lexer	*lex_last(t_lexer *lst)
{
	if (lst == NULL)
		return (lst);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	add_token(t_lexer **lst, t_lexer *new)
{
	t_lexer	*tmp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		tmp = lex_last(*lst);
		new->prev = tmp;
		tmp->next = new;
	}
}

void	print_lex(t_lexer *lexer)
{
	t_lexer	*tmp;

	tmp = lexer;
	while (tmp)
	{
		printf("content = %s/// token = %d\n", tmp->content, tmp->token);
		tmp = tmp->next;
	}
}

void	new_ope_token(t_lexer **lexer, char **cmd)
{
	if (**cmd == '>')
	{
		if (*(*cmd + 1) == '>')
		{
			add_token(lexer, new_token(ft_strdup(">>"), D_GREAT));
			(*cmd)++;
		}
		else
			add_token(lexer, new_token(ft_strdup(">"), GREAT));
	}
	else if (**cmd == '<')
	{
		if (*(*cmd + 1) == '<')
		{
			add_token(lexer, new_token(ft_strdup("<<"), D_LESS));
			(*cmd)++;
		}
		else
			add_token(lexer, new_token(ft_strdup("<"), LESS));
	}
	else if (**cmd == '|')
		add_token(lexer, new_token(ft_strdup("|"), PIPE));
	(*cmd)++;
}
