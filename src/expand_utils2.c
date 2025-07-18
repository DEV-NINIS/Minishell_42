/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:09:32 by ael-fari          #+#    #+#             */
/*   Updated: 2025/07/02 21:09:34 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_expand_values(t_expand_values *expand_values)
{
	if (!expand_values)
		return ;
	free(expand_values->joined);
	free(expand_values->expanded);
	free(expand_values->tmp2);
}
