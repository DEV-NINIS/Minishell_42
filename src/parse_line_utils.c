/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:01:23 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/03 17:01:28 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == NODE_COMMAND)
		free_string_array(ast->cmd.args);
	else if (ast->type == NODE_PIPE)
	{
		free_ast(ast->pipe.left);
		free_ast(ast->pipe.right);
	}
	else if (ast->type == NODE_REDIRECT)
	{
		free_ast(ast->redirect.command);
		free(ast->redirect.filename);
	}
	free(ast);
}

void	advance(t_lexer **lexer)
{
	if (*lexer)
		*lexer = (*lexer)->next;
}

t_ast	*parse(t_lexer **tokens)
{
	return (parse_pipeline(tokens));
}
