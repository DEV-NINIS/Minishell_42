/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:24:24 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/18 17:24:41 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char			content;
	long int		nb;

	if (fd >= 0)
	{
		nb = n;
		if (nb < 0)
		{
			write(fd, "-", 1);
			nb *= -1;
		}
		if (nb < 10)
		{
			content = nb + '0';
			write(fd, &content, 1);
		}
		else
		{
			ft_putnbr_fd(nb / 10, fd);
			ft_putnbr_fd(nb % 10, fd);
		}
	}
}
