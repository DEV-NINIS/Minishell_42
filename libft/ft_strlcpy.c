/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:28:05 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:28:34 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t siz)
{
	size_t	count;

	count = 0;
	if (siz == 0)
		return (ft_strlen(src));
	while (count < siz - 1 && src[count] != '\0')
	{
		dst[count] = *(char *)&src[count];
		count++;
	}
	dst[count] = '\0';
	return (ft_strlen(src));
}
