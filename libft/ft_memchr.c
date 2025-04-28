/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:34:36 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:35:49 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s_copy;
	size_t			count;

	count = 0;
	s_copy = (unsigned char *)s;
	while (count < n)
	{
		if (*s_copy == (unsigned char)c)
		{
			return (s_copy);
		}
		s_copy++;
		count++;
	}
	return (NULL);
}
