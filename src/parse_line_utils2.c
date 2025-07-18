/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:03:31 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/03 17:03:36 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**parse_args(t_lexer **lexer)
{
	int		count;
	int		i;
	char	**args;
	t_lexer	*tmp;

	count = 0;
	tmp = *lexer;
	while (tmp && tmp->token == WORD)
	{
		count++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (*lexer && (*lexer)->token == WORD)
	{
		args[i++] = strdup((*lexer)->content);
		advance(lexer);
	}
	args[i] = NULL;
	return (args);
}

t_ast	*parse_redirects(t_lexer **lexer)
{
	t_ast	*cmd;

	cmd = NULL;
	if (*lexer && (*lexer)->token == WORD)
	{
		cmd = malloc(sizeof(t_ast));
		if (!cmd)
			return (NULL);
		cmd->type = NODE_COMMAND;
		cmd->cmd.args = parse_args(lexer);
	}
	while (*lexer && ((*lexer)->token == GREAT || (*lexer)->token == LESS
			|| (*lexer)->token == D_GREAT || (*lexer)->token == D_LESS))
	{
		cmd = handle_redirect(lexer, cmd);
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}

t_ast	*handle_redirect(t_lexer **lexer, t_ast *cmd)
{
	t_ast	*redir;
	t_token	redir_type;

	redir_type = (*lexer)->token;
	advance(lexer);
	if (!*lexer || (*lexer)->token != WORD)
	{
		fprintf(stderr, "Syntax error: expected filename\n");
		return (NULL);
	}
	redir = malloc(sizeof(t_ast));
	if (!redir)
		return (NULL);
	redir->type = NODE_REDIRECT;
	redir->redirect.redirect_type = redir_type;
	redir->redirect.filename = strdup((*lexer)->content);
	redir->redirect.command = cmd;
	advance(lexer);
	return (redir);
}

t_ast	*parse_pipeline(t_lexer **lexer)
{
	t_ast	*left;
	t_ast	*right;

	if (*lexer && (*lexer)->token == PIPE)
		return (fprintf(stderr, "Syntax error: unexpected token `|'\n"), NULL);
	left = parse_redirects(lexer);
	if (!left)
		return (NULL);
	while (*lexer && (*lexer)->token == PIPE)
	{
		advance(lexer);
		if (!*lexer || (*lexer)->token == PIPE)
			return (free_ast(left), fprintf(stderr,
					"Syntax error: unexpected token `|'\n"), NULL);
		right = parse_redirects(lexer);
		if (!right)
			return (free_ast(left), NULL);
		left = create_pipe_node(left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*pipe_node;

	pipe_node = malloc(sizeof(t_ast));
	if (!pipe_node)
		return (NULL);
	pipe_node->type = NODE_PIPE;
	pipe_node->pipe.left = left;
	pipe_node->pipe.right = right;
	return (pipe_node);
}
