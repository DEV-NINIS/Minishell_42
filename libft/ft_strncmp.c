/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:33:40 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:34:19 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	count;

	count = 0;
	if (n <= 0)
		return (0);
	while (count < n && (s1[count] != '\0'
			|| s2[count] != '\0'))
	{
		if (s1[count] < s2[count] || s1[count] > s2[count])
		{
			return ((unsigned char)s1[count] - (unsigned char)s2[count]);
		}
		count++;
	}
	return (0);
}
