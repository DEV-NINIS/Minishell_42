/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:55:14 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:56:25 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	int		count;
	int		count2;

	count2 = 0;
	count = 0;
	result = malloc(sizeof(char) + sizeof(char)
			* ft_strlen(s1) + sizeof(char) * (ft_strlen(s2)));
	if (!result)
		return (NULL);
	while (s1[count] != '\0')
	{
		result[count] = s1[count];
		count++;
	}
	while (s2[count2] != '\0')
	{
		result[count] = s2[count2];
		count++;
		count2++;
	}
	result[count] = '\0';
	return (result);
}
