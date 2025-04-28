/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:45:03 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:54:48 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	allocate_memory(char **result, char const *s,
							unsigned int start, size_t len)
{
	if (ft_strlen(s) - start == 1)
		*result = malloc(sizeof(char) * 2);
	else if (start > ft_strlen(s))
		*result = malloc(sizeof(char));
	else if (ft_strlen(s) < len)
		*result = malloc(sizeof(char) * (ft_strlen(s) + 1));
	else
		*result = malloc(sizeof(char) * (len + 1));
	if (!*result)
		return (0);
	return (1);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	count;
	size_t	count2;
	char	*result;

	count = 0;
	count2 = 0;
	if (!allocate_memory(&result, s, start, len))
		return (NULL);
	while (count2 < len && s[count] != '\0')
	{
		if (count >= start)
		{
			result[count2] = s[count];
			count2++;
		}
		count++;
	}
	result[count2] = '\0';
	return (result);
}
