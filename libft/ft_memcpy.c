/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:25:40 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:26:13 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	count;
	char	*ptr_dest;
	char	*ptr_src;

	if (!dest && !src)
		return (0);
	ptr_src = (char *)src;
	ptr_dest = (char *)dest;
	count = 0;
	while (count < n)
	{
		((unsigned char *)ptr_dest)[count] = ((unsigned char *)ptr_src)[count];
		count++;
	}
	return (dest);
}
