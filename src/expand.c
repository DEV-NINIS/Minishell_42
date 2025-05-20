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

// Cherche la valeur d'une variable d'environnement dans la liste
char *get_env_value(char *key, t_env *env)
{
    while (env)
    {
        if (ft_strncmp(env->current_key, key, ft_strlen(env->current_key)) == 0)
            return ft_strdup(env->current_value);
        env = env->next;
    }
    return ft_strdup(""); // Si non trouvée, retourner string vide
}
char *expand_exit_status(int exit_status)
{
    return ft_itoa(exit_status);
}
char *get_env_key(const char *str, int *i)
{
    int start = *i;
    int len = 0;

    if (str[*i] == '?')
    {
        (*i)++;
        return ft_strdup("?");
    }

    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
    {
        (*i)++;
        len++;
    }

    return ft_substr(str, start, len);
}

// Effectue l'expansion d'une seule variable
char *expand_variable(const char *str, int *i, t_env *env, int exit_status)
{
    char *key;
    char *value;

    (*i)++; // Skip $
    key = get_env_key(str, i);

    if (ft_strncmp(key, "?", ft_strlen(key)) == 0)
        value = expand_exit_status(exit_status);
    else
        value = get_env_value(key, env);

    free(key);
    return value;
}

char *expand_string(const char *input, t_env *env, int exit_status)
{
    char *result = ft_calloc(1, sizeof(char));
    int i = 0;
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (input[i])
    {
        if (input[i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (input[i] == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        else if (input[i] == '$' && !in_single_quote)
        {
            char *expanded = expand_variable(input, &i, env, exit_status);
            char *tmp = ft_strjoin(result, expanded);
            free(result);
            free(expanded);
            result = tmp;
            continue;
        }
        else
        {
            char tmp[2] = {input[i], '\0'};
            char *joined = ft_strjoin(result, tmp);
            free(result);
            result = joined;
        }
        i++;
    }
    return result;
}

void expand_cmd_args(t_cmd *cmd, t_env *env, int exit_status)
{
    int i = 0;

    while (cmd && cmd->args && cmd->args[i])
    {
        char *expanded = expand_string(cmd->args[i], env, exit_status);
        free(cmd->args[i]);
        cmd->args[i] = expanded;
        i++;
    }

    if (cmd->infile)
    {
        char *expanded = expand_string(cmd->infile, env, exit_status);
        free(cmd->infile);
        cmd->infile = expanded;
    }

    if (cmd->outfile)
    {
        char *expanded = expand_string(cmd->outfile, env, exit_status);
        free(cmd->outfile);
        cmd->outfile = expanded;
    }
}

void expand_all_commands(t_cmd *cmds, t_env *env, int exit_status)
{
    while (cmds)
    {
        expand_cmd_args(cmds, env, exit_status);
        cmds = cmds->next;
    }
}
