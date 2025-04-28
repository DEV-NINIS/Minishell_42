/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:42:15 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 16:42:37 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	count;
	int	result;

	sign = 1;
	count = 0;
	result = 0;
	while (nptr[count] == ' ' || (nptr[count] >= 9 && nptr[count] <= 13))
		count++;
	if (nptr[count] == '+' || nptr[count] == '-')
	{
		if (nptr[count] == '-')
			sign *= -1;
		count++;
	}
	while (nptr[count] >= 48 && nptr[count] <= 57)
	{
		result = result * 10 + (nptr[count] - '0');
		count++;
	}
	return (result * sign);
}
