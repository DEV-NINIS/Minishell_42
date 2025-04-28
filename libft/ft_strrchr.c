/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:32:08 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:33:16 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str_copy;

	str_copy = (char *)s;
	str_copy += ft_strlen(s);
	while (str_copy >= s)
	{
		if (*str_copy == (char)c)
		{
			return (str_copy);
		}
		str_copy--;
	}
	return (NULL);
}
