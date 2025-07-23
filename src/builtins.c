/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 18:20:41 by ael-fari          #+#    #+#             */
/*   Updated: 2025/04/25 18:22:42 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <../inc/minishell.h>

void	builtin_env(t_env **env)
{
	while ((*env)->previous != NULL)
		(*env) = (*env)->previous;
	while ((*env)->next != NULL)
	{
		printf("%s=%s\n", (*env)->current_key, (*env)->current_value);
		(*env) = (*env)->next;
	}
	printf("%s=%s\n", (*env)->current_key, (*env)->current_value);
}

int	builtin_cd(char **args, t_env **env)
{
	char	*target_dir;
	char	*home_dir;

	if (!args[1])
	{
		home_dir = get_env_value("HOME", env);
		if (!home_dir)
		{
			write(2, "cd: HOME not set\n", 18);
			return (0);
		}
		target_dir = home_dir;
	}
	else
		target_dir = args[1];
	if (chdir(target_dir) != 0)
	{
		write(2, "cd: ", 4);
		perror(target_dir);
	}
	return (1);
}

static char	*extract_key(char *arg)
{
	char	*eq;
	size_t	len;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (NULL);
	len = eq - arg;
	return (ft_substr(arg, 0, len));
}

static char	*extract_value(char *arg)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (NULL);
	return (ft_strdup(eq + 1));
}

t_env	**builtin_export(t_env **env, t_cmd **args)
{
	int		i;
	char	*key;
	char	*value;

	i = 1;
	while ((*args)->args[i])
	{
		if (ft_strchr((*args)->args[i], '='))
		{
			key = extract_key((*args)->args[i]);
			value = extract_value((*args)->args[i]);
			if (key && value)
				env = add_env(env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (env);
}
