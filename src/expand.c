/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:13:13 by ael-fari          #+#    #+#             */
/*   Updated: 2025/05/20 16:13:32 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_env(t_env **env)
{
	while ((*env)->previous != NULL)
		(*env) = (*env)->previous;
}

char	*get_env_value(char *key, t_env **env)
{
	char	*result;

	while ((*env)->next)
	{
		if (ft_strcmp((*env)->current_key, key) == 0)
		{
			result = ft_strdup((*env)->current_value);
			return (reset_env(env), result);
		}
		(*env) = (*env)->next;
	}
	if (ft_strcmp((*env)->current_key, key) == 0)
	{
		result = ft_strdup((*env)->current_value);
		return (reset_env(env), result);
	}
	return (reset_env(env), ft_strdup(""));
}

char	*expand_exit_status(int exit_status)
{
	return (ft_itoa(exit_status));
}

char	*get_env_key(const char *str, int *i)
{
	int	start;
	int	len;

	start = *i;
	len = 0;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
	{
		(*i)++;
		len++;
	}
	return (ft_substr(str, start, len));
}

char	*expand_variable(const char *str, int *i, t_env **env, int exit_status)
{
	char	*key;
	char	*value;

	(*i)++;
	key = get_env_key(str, i);
	if (ft_strcmp(key, "?") == 0)
		value = expand_exit_status(exit_status);
	else
		value = get_env_value(key, env);
	reset_env(env);
	free(key);
	return (value);
}
