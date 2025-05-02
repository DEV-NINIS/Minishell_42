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

void    builtin_pwd()
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd");
}

void    builtin_env(t_env **env)
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

int	builtin_unset_test_equal(t_env **env, char *value_delete, int count_forward, t_env *temp)
{
    if (!ft_strncmp((const char *)((*env)->current_key), (const char *)(value_delete), ft_strlen((*env)->current_key)))
    {
        free((*env)->current_key);
        free((*env)->current_value);
        temp = (*env);
        if (count_forward == 0)
        {
            (*env) = (*env)->next;
            (*env)->previous = NULL;
        }
        else
        {
            if ((*env)->next == NULL)
            {
                (*env) = (*env)->previous;
                (*env)->next = NULL;
            }
            else
            {
                (*env)->previous->next = (*env)->next;
                (*env)->next->previous = (*env)->previous;
                (*env) = (*env)->previous;
            }
            if (count_forward == 1)
                (*env)->previous = NULL;
        }
        free(temp);
        return (1);
    }
    return (0);
}

void	builtin_unset(t_env **env, char *value_delete)
{
    t_env   *temp;
    int     count_forward;

    temp = NULL;
    count_forward = 0;
    while ((*env)->previous != NULL)
        (*env) = (*env)->previous;
    
    while ((*env)->next != NULL)
    {
        if (builtin_unset_test_equal(env, value_delete, count_forward, temp))
            return ;
        (*env) = (*env)->next;
        count_forward++;
    }
    builtin_unset_test_equal(env, value_delete, count_forward, temp);
}