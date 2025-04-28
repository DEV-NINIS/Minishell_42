/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:27:35 by ael-fari          #+#    #+#             */
/*   Updated: 2024/12/04 16:27:37 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*result;
	char	*a;
	size_t	count;

	if (size && (nmemb > __SIZE_MAX__ / size))
		return (NULL);
	result = malloc(nmemb * (size));
	if (!result)
		return (NULL);
	count = 0;
	a = (char *)result;
	while (count != nmemb * size)
	{
		*a = '\0';
		a++;
		count++;
	}
	return (result);
}

size_t	ft_strlen(char *str)
{
	int	count;

	count = 0;
	while (str && str[count])
		count++;
	return (count);
}

char	*copy_old_content(char **dest, char **swap, int *count,
		size_t *size_dst)
{
	if (*dest != NULL)
	{
		while ((*dest)[*size_dst] != '\0')
			(*size_dst)++;
		(*size_dst)++;
		*swap = ft_calloc(sizeof(char), (*size_dst + 1));
		if (!*swap)
			return (NULL);
		while ((*dest)[*count] != '\0')
		{
			(*swap)[*count] = (*dest)[*count];
			(*count)++;
		}
		(*swap)[*count] = '\0';
	}
	return (*swap);
}

char	*push_back_content(char *dest, char *content, char content_stop,
		int *count_buffer)
{
	size_t	size_dst;
	int		count;
	char	*swap;
	char	*new_dest;

	count = 0;
	size_dst = 0;
	swap = NULL;
	new_dest = NULL;
	copy_old_content(&dest, &swap, &count, &size_dst);
	new_dest = ft_calloc(sizeof(char), (size_dst + ft_strlen(content) + 1));
	count = 0;
	if (!new_dest)
		return (free(swap), NULL);
	while (swap && swap[count] != '\0')
	{
		new_dest[count] = swap[count];
		count++;
	}
	*count_buffer = count;
	while (*content && *content != content_stop)
		new_dest[(*count_buffer)++] = *content++;
	return (free(swap), free(dest), new_dest);
}
