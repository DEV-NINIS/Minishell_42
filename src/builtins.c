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

void	builtin_unset(t_env **env, char *value_delete)
{
    t_env   *temp;
    int     count_forward;

    count_forward = 0;
    while ((*env)->previous != NULL)
        (*env) = (*env)->previous;
    
    while ((*env)->next != NULL)
    {
        if ((*env)->current_key == value_delete)
        {
            free((*env)->current_key);
            free((*env)->current_value);
            temp = (*env);
            if (count_forward == 0)
                (*env) = (*env)->next;
            else
            {
                if ((*env)->next == NULL)
                    (*env) = (*env)->previous;
                else
                    (*env)->next->previous = (*env)->previous;
            }
            free(temp);
        }
        (*env) = (*env)->next;
        count_forward++;
    }
}