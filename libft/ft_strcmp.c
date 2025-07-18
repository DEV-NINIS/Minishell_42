/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quelefev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:30:29 by quelefev          #+#    #+#             */
/*   Updated: 2025/07/10 16:30:34 by quelefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	count;

	count = 0;
	while ((s1[count] != '\0' || s2[count] != '\0'))
	{
		if (s1[count] < s2[count] || s1[count] > s2[count])
		{
			return ((unsigned char)s1[count] - (unsigned char)s2[count]);
		}
		count++;
	}
	return (0);
}
