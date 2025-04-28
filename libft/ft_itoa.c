/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:01:47 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 17:19:48 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_nb_characters(int nb)
{
	long int	n;
	size_t		result;

	result = 1;
	n = nb;
	if (n < 0)
	{
		result++;
		n *= -1;
	}
	while (n >= 10)
	{
		n /= 10;
		result++;
	}
	return (result);
}

static int	allocate(char **result, int *n, size_t *size_result)
{
	if (*n == 0)
	{
		*result = malloc(sizeof(char) * 2);
		(*result)[0] = '0';
		(*result)[1] = '\0';
		return (2);
	}
	*size_result = get_nb_characters(*n);
	*result = malloc(sizeof(char) * (*size_result + 1));
	if (!*result)
		return (0);
	return (1);
}

static void	set_sign(char **result, long int *nb,
					int *count_sign, size_t *count)
{
	if (*nb < 0)
	{
		(*result)[*count] = '-';
		(*nb) *= -1;
		(*count_sign)++;
		(*count)++;
	}
}

char	*ft_itoa(int n)
{
	char		*result;
	size_t		size_result;
	size_t		count;
	int			count_sign;
	long int	nb;

	count_sign = 0;
	count = 0;
	nb = n;
	result = 0;
	size_result = 0;
	if (!allocate(&result, &n, &size_result))
		return (NULL);
	if (n == 0)
		return (result);
	set_sign(&result, &nb, &count_sign, &count);
	while (nb != 0)
	{
		result[(size_result - 1) - (count - count_sign)] = nb % 10 + '0';
		nb /= 10;
		count++;
	}
	result[size_result] = '\0';
	return (result);
}
