/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:42:57 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:43:53 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*result;

	if (size && (nmemb > __SIZE_MAX__ / size))
		return (NULL);
	result = malloc(nmemb * (size));
	if (!result)
		return (NULL);
	ft_bzero(result, nmemb * size);
	return (result);
}
