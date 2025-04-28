/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:38:08 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:41:58 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	len_little;
	char	*big_ptr;

	if (!*little)
		return ((char *) big);
	if (len == 0)
		return (NULL);
	len_little = ft_strlen(little);
	big_ptr = (char *)big;
	while (*big_ptr && len >= len_little)
	{
		if (ft_strncmp(big_ptr, little, len_little) == 0)
			return (big_ptr);
		big_ptr++;
		len--;
	}
	return (NULL);
}
