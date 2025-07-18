/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:04:31 by ael-fari          #+#    #+#             */
/*   Updated: 2025/06/25 16:04:33 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	expand_string_in_while(const char *input, t_env **env, int exit_status,
		t_expand_values *v)
{
	if (input[v->i] == '\'' && !v->in_double_quote)
		v->in_single_quote = !v->in_single_quote;
	else if (input[v->i] == '"' && !v->in_single_quote)
		v->in_double_quote = !v->in_double_quote;
	else if (input[v->i] == '$' && !v->in_single_quote)
		return (expand_handle_dollar(input, env, exit_status, v));
	else
		expand_handle_regular_char(input, v);
	return (1);
}

int	expand_handle_dollar(const char *input, t_env **env, int exit_status,
		t_expand_values *v)
{
	v->expanded = expand_variable(input, &v->i, env, exit_status);
	v->tmp2 = ft_strjoin(v->result, v->expanded);
	free(v->result);
	free(v->expanded);
	v->result = v->tmp2;
	return (0);
}

void	expand_handle_regular_char(const char *input, t_expand_values *v)
{
	v->tmp[0] = input[v->i];
	v->tmp[1] = '\0';
	v->joined = ft_strjoin(v->result, v->tmp);
	free(v->result);
	v->result = ft_strdup(v->joined);
	free(v->joined);
}

char	*expand_string(const char *input, t_env **env, int exit_status)
{
	t_expand_values	expand_values;

	expand_values.i = 0;
	expand_values.in_single_quote = 0;
	expand_values.in_double_quote = 0;
	expand_values.result = ft_calloc(1, sizeof(char));
	while (input[expand_values.i])
	{
		if (!expand_string_in_while(input, env, exit_status, &expand_values))
			continue ;
		expand_values.i++;
	}
	return (expand_values.result);
}

void	expand_lexer(t_lexer *lexer, t_env **env, int exit_status)
{
	t_lexer	*current;
	t_lexer	*head;
	char	*expanded;

	head = lexer;
	current = lexer;
	while (current)
	{
		if (current->token == WORD)
		{
			expanded = expand_string(current->content, env, exit_status);
			free(current->content);
			current->content = NULL;
			current->content = expanded;
		}
		current = current->next;
	}
	lexer = head;
}
