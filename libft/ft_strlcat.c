/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:29:04 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:29:25 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t	src_len;
	size_t	dest_len;
	size_t	count;
	size_t	count2;

	dest_len = 0;
	while (dst[dest_len] != '\0')
		dest_len++;
	src_len = 0;
	while (src[src_len] != '\0')
		src_len++;
	if (siz <= dest_len)
		return (siz + src_len);
	count = dest_len;
	count2 = 0;
	while (src[count2] != '\0' && count < siz - 1)
	{
		dst[count] = src[count2];
		count++;
		count2++;
	}
	dst[count] = '\0';
	return (dest_len + src_len);
}
