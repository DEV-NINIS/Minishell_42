/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:03:56 by ael-fari          #+#    #+#             */
/*   Updated: 2025/05/02 16:04:13 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_lexer	*handle_line(char *line)
{
    int     count;
    char    *word;
    t_lexer *result;

    result = NULL;
    count = 0;
    while (line[count])
    {
        while (line[count] == ' ')
            count++;
        if (line[count])
            word = cpy_word(line, &count);
        if (!result)
        {
            result = malloc(sizeof(t_lexer));
            if (!result)
                return (NULL);
            result->previous = NULL;
        }
        else
        {
            result->next = malloc(sizeof(t_lexer));
            if (!result->next)
                return (NULL);
            result->next->previous = result;
            result = result->next;
        }
        result->next = NULL;
        result->content = word;
    }
    return (result);
}
char	*cpy_word(char *line, int *count)
{
    char    *result;
    int     size_word;
    int     count2;

    count2 = -1;
    size_word = 0;
    if (line[*count] == '\'' || line[*count] == '\"')
    {
        while (line[*count + size_word] && (line[*count + size_word] != '\'' || line[*count + size_word] != '\"'))
        {
            size_word++;
            if (line[*count + size_word] && (line[*count + size_word] == '\'' || line[*count + size_word] == '\"'))
                size_word++;
        }
    }
    else
        while (line[*count + size_word] && line[*count + size_word] != ' ')
            size_word++;
    result = ft_calloc(sizeof(char), size_word + 1);
    if (!result)
        return (NULL);
    while (++count2 < size_word)
        result[count2] = line[(*count)++];
    return (result);
}