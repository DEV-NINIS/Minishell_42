/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:49:53 by ael-fari          #+#    #+#             */
/*   Updated: 2025/06/23 18:49:56 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && is_echo_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

static void	unset_head_node(t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	*env = (*env)->next;
	if (*env)
		(*env)->previous = NULL;
	free(tmp->current_key);
	free(tmp->current_value);
	free(tmp);
}

static void	unset_middle_node(t_env *prev, t_env *current)
{
	prev->next = current->next;
	if (current->next)
		current->next->previous = prev;
	free(current->current_key);
	free(current->current_value);
	free(current);
}

static void	remove_key_from_env(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->current_key, key,
				ft_strlen(current->current_key)) == 0)
		{
			if (!prev)
				unset_head_node(env);
			else
				unset_middle_node(prev, current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

// void	builtin_unset(t_env **env, char *key)
// {
// 	if (!key || !*key || ft_strchr(key, '='))
// 	{
// 		write(2, "unset: invalid identifier\n", 26);
// 		return ;
// 	}
// 	remove_key_from_env(env, key);
// }

void	builtin_unset(t_env **env, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!args[i][0] || ft_strchr(args[i], '='))
		{
			write(2, "unset: invalid identifier\n", 26);
			i++;
			continue ;
		}
		remove_key_from_env(env, args[i]);
		while ((*env)->previous != NULL)
			(*env) = (*env)->previous;
		i++;
	}
}
