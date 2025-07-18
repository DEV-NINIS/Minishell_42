/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:42:51 by ael-fari          #+#    #+#             */
/*   Updated: 2025/06/23 19:43:03 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->current_key = ft_strdup(key);
	new->current_value = ft_strdup(value);
	new->next = NULL;
	new->previous = NULL;
	return (new);
}

t_env	**add_env_utils(t_env **env, char *key, char *value)
{
	t_env	*new;
	t_env	*cur;

	new = create_new_env_node(key, value);
	if (!new)
		return (NULL);
	if (!*env)
	{
		*env = new;
		return (env);
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	new->previous = cur;
	return (env);
}

t_env	**add_env(t_env **env, char *key, char *value)
{
	t_env	*cur;

	if (!env || !key || !value)
		return (env);
	cur = *env;
	while (cur)
	{
		if (ft_strncmp(cur->current_key, key, ft_strlen(key)) == 0)
		{
			free(cur->current_value);
			cur->current_value = ft_strdup(value);
			return (env);
		}
		cur = cur->next;
	}
	return (add_env_utils(env, key, value));
}

int	set_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->current_key, key, ft_strlen(tmp->current_key)) == 0)
		{
			free(tmp->current_value);
			tmp->current_value = ft_strdup(value);
			return (0);
		}
		tmp = tmp->next;
	}
	add_env(env, ft_strdup(key), ft_strdup(value));
	return (1);
}

void	free_env(t_env **env)
{
	t_env	*temp;

	temp = NULL;
	while ((*env)->previous != NULL)
		(*env) = (*env)->previous;
	while ((*env)->next != NULL)
	{
		free((*env)->current_key);
		free((*env)->current_value);
		temp = (*env);
		(*env) = (*env)->next;
		free(temp);
	}
	free((*env)->current_key);
	free((*env)->current_value);
	free(*env);
}
