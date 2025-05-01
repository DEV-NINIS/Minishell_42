/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:02:36 by ael-fari          #+#    #+#             */
/*   Updated: 2025/04/27 20:03:00 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int		ft_strlen_key(char *str_complete_line)
{
    int count;

    count = 0;
    while (str_complete_line[count] != '=')
    {
        count++;
    }
    return (count);
}

int		ft_strlen_value(char *str_complete_line)
{
    int count;
    int count_result;

    count_result = 0;
    count = 0;
    while (str_complete_line[count] != '=')
        count++;
    count++;
    while (str_complete_line[count])
    {
        count++;
        count_result++;
    }
    return (count_result);
}

void	*init_env_list_cpy_txt(t_env **current, char **env, int count)
{
    int     count3;
    int     count2;

    (*current)->current_key = ft_calloc(sizeof(char), ft_strlen_key(env[count]) + 1);
    if (!(*current)->current_key)
        return (NULL);
    (*current)->current_value = ft_calloc(sizeof(char), ft_strlen_value(env[count]) + 1);
    if (!(*current)->current_value)
        return (NULL);
    count2 = -1;
    while (env[count][++count2] != '=')
        (*current)->current_key[count2] = env[count][count2];
    count3 = -1;
    while (env[count][++count2])
        (*current)->current_value[++count3] = env[count][count2];
    return ((void*)(*current));
}

t_env	*init_env_list(char **env)
{
    t_env   *temp;
    t_env   *current;
    int     count;

    current = malloc(sizeof(t_env));
    if (!current)
        return (NULL);
    current->previous = NULL;
    count = -1;
    while (env[++count])
    {
        init_env_list_cpy_txt(&current, env, count);
        if (!env[count + 1])
            break;
        current->next = malloc(sizeof(t_env));
        if (!current->next)
            return (NULL);
        temp = current;
        current = current->next;
        current->previous = temp;
        current->next = NULL;
    }
    while (current->previous != NULL)
        current = current->previous;
    return (current);
}
int		add_env(t_env **env, char *key, char *value)
{
    t_env   *temp;

    while ((*env)->next != NULL)
        (*env) = (*env)->next;
    temp = (*env);
    (*env)->next = malloc(sizeof(t_env));
    (*env) = (*env)->next;
    (*env)->next = NULL;
    (*env)->previous = temp;
    (*env)->current_key = ft_strdup((const char *)(key));
    if (!(*env)->current_key)
        return (0);
    (*env)->current_value = ft_strdup((const char *)(value));
    if (!(*env)->current_value)
        return (0);
    while ((*env)->previous != NULL)
        (*env) = (*env)->previous;
    return (1);
}

void	free_env(t_env **env)
{
    while ((*env)->next != NULL)
        (*env) = (*env)->previous;
    while ((*env)->previous != NULL)
    {
        printf("previous is -> %p\n", (*env)->previous);
        free((*env)->current_key);
        free((*env)->current_value);
        (*env) = (*env)->previous;
    }
}

