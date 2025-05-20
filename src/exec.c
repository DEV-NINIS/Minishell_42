/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:05:24 by ael-fari          #+#    #+#             */
/*   Updated: 2025/05/05 16:06:13 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*add_path_build(char *word_command, char *path_content)
{
	char	*temp;
	int		count;
	int		count2;

	count = -1;
	count2 = -1;
	temp = ft_calloc(sizeof(char), ft_strlen(word_command) + 13);
	if (!temp)
		return (NULL);
	while (path_content[++count])
		temp[count] = path_content[count];
	while (word_command[++count2])
	{
		temp[count] = word_command[count2];
		count++;
	}
	return (temp);
}

char	*get_my_line_here_doc(void)
{
	char	*buf;

	buf = NULL;
	if (buf)
	{
		free(buf);
		buf = NULL;
	}
	buf = readline("heredoc > ");
	return (buf);
}

void	*push_back_list(char *content, t_char_list **list)
{
	t_char_list	*temp;

	temp = (*list);
	if (!(*list))
	{
		(*list) = malloc(sizeof(t_char_list));
		if (!(*list))
			return (NULL);
		(*list)->content = ft_strdup(content);
		if (!((*list)->content))
			return (NULL);
		(*list)->next = NULL;
		(*list)->previous = NULL;
	}
	else
	{
		temp->next = malloc(sizeof(t_char_list));
		if (!temp->next)
			return (NULL);
		temp->next->previous = temp;
		temp = temp->next;
		temp->content = ft_strdup(content);
		if (!temp->content)
			return (NULL);
		temp->next = NULL;
	}
	return (NULL);
}

void	free_char_list(t_char_list **list)
{
	while ((**list).previous != NULL)
		(*list) = (**list).previous;
	while ((**list).next != NULL)
		free((**list).content);
}

int	convert_chain_to_star_char_utils(t_char_list **list_chain, char **result,
		int *count)
{
	int	size_y;

	result[*count] = ft_calloc(sizeof(char), ft_strlen((*list_chain)->content)
			+ 1);
	if (!result[*count])
		return (-1);
	size_y = -1;
	while ((*list_chain)->content[++(size_y)])
		result[*count][size_y] = (*list_chain)->content[size_y];
	printf("just setted -> %s\n", result[*count]);
	(*list_chain) = (*list_chain)->next;
	(*count)++;
	return (1);
}
char	**convert_chain_to_star_char(t_char_list **list_chain)
{
	char	**result;
	int		size_y;
	int		count;

	size_y = 0;
	count = 0;
	while ((*list_chain)->previous != NULL)
		(*list_chain) = (*list_chain)->previous;
	while ((*list_chain)->next != NULL)
	{
		(*list_chain) = (*list_chain)->next;
		size_y++;
	}
	size_y++;
	while ((*list_chain)->previous != NULL)
		(*list_chain) = (*list_chain)->previous;
	result = malloc(sizeof(char *) * (size_y + 1));
	if (!result)
		return (NULL);
	result[size_y] = NULL;
	while ((*list_chain)->next != NULL)
		convert_chain_to_star_char_utils(list_chain, result, &count);
	convert_chain_to_star_char_utils(list_chain, result, &count);
	return (result);
}

int	create_heredoc_pipe(const char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);   // fermer le write-end
	return (pipefd[0]); // retourner le read-end pour dup2
}

void	read_line_heredoc(t_char_list **list, char *arg_end)
{
	char	*rl;

	while ((rl = get_my_line_here_doc()) && !ft_strncmp(rl, arg_end,
			ft_strlen(rl)))
	{
		push_back_list(rl, list);
	}
}

void	execute_simple_command(t_cmd *cmd, t_env **envp)
{
	t_sig	sig;
	int		heredoc_fd;
	int		flags;
	char	**en;

	expand_all_commands(cmd, *envp, 0);
	sig.pid = fork();
	if (sig.pid == -1 && envp)
		return ;
	if (sig.pid == 0)
	{
		if (cmd->heredoc)
		{
			heredoc_fd = create_heredoc_pipe(cmd->heredoc_delimiter);
			if (heredoc_fd == -1)
				exit(EXIT_FAILURE);
			dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		else if (cmd->infile)
		{
			sig.sigint = open(cmd->infile, O_RDONLY);
			if (sig.sigint == -1)
			{
				printf("Fail");
				exit(EXIT_FAILURE);
			}
			dup2(sig.sigint, STDIN_FILENO);
			close(sig.sigint);
		}
		if (cmd->outfile)
		{
			flags = O_WRONLY | O_CREAT;
			flags |= (cmd->append) ? O_APPEND : O_TRUNC;
			sig.sigquit = open(cmd->outfile, flags, 0644);
			if (sig.sigquit == -1)
			{
				printf("Fail");
				exit(EXIT_FAILURE);
			}
			dup2(sig.sigquit, STDOUT_FILENO);
			close(sig.sigquit);
		}
		en = convert_l_env_to_char_env(envp);
		execve(add_path_build(cmd->args[0], "/usr/bin/"), cmd->args, en);
		exit(EXIT_FAILURE);
	}
	else
		waitpid(sig.pid, &sig.exit_status, 0);
}

int	has_final_heredoc(t_cmd *cmds)
{
	t_cmd	*last;

	last = cmds;
	while (last && last->next)
		last = last->next;
	return (last && last->heredoc);
}

void	execute_pipeline(t_cmd *cmds, t_env **env)
{
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	t_cmd	*current;
	int		heredoc_fd;
	int		in;
	int		flags;
	int		out;

	prev_fd = -1;
	current = cmds;
	while (current)
	{
		expand_all_commands(current, *env, 0);
		if (current->next && pipe(fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) // child
		{
			// ⛓️ Lecture du pipe précédent
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			// 🧾 Heredoc > infile si présent
			if (current->heredoc)
			{
				heredoc_fd = create_heredoc_pipe(current->heredoc_delimiter);
				if (heredoc_fd == -1)
					exit(EXIT_FAILURE);
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
			else if (current->infile)
			{
				in = open(current->infile, O_RDONLY);
				if (in == -1)
				{
					perror("open infile");
					exit(EXIT_FAILURE);
				}
				dup2(in, STDIN_FILENO);
				close(in);
			}
			// 📤 Redirection output ou pipe
			if (current->next)
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			if (current->outfile)
			{
				flags = O_CREAT | O_WRONLY | (current->append ? O_APPEND : O_TRUNC);
				out = open(current->outfile, flags, 0644);
				if (out == -1)
				{
					perror("open outfile");
					exit(EXIT_FAILURE);
				}
				dup2(out, STDOUT_FILENO);
				close(out);
			}
			// 👟 Lancement de la commande
			execve(add_path_build(current->args[0], "/usr/bin/"), current->args,
				convert_l_env_to_char_env(env));
			perror("execve");
			exit(EXIT_FAILURE);
		}
		// 🧹 Parent - nettoyage
		if (prev_fd != -1)
			close(prev_fd);
		if (current->next)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		current = current->next;
	}
	// 🧼 Attend tous les enfants
	while (wait(NULL) > 0)
		;
}

int	convert_l_env_to_char_env_utils(t_env **env, char **result, int *count)
{
	int	size_y;
	int	count2;

	result[*count] = ft_calloc(sizeof(char), ft_strlen((*env)->current_key)
			+ ft_strlen((*env)->current_value) + 2);
	if (!result[*count])
		return (-1);
	size_y = -1;
	count2 = -1;
	while ((*env)->current_key[++(size_y)])
		result[*count][size_y] = (*env)->current_key[size_y];
	result[*count][size_y] = '=';
	while ((*env)->current_value[++(count2)] && ++size_y > 0)
		result[*count][(size_y)] = (*env)->current_value[(count2)];
	(*env) = (*env)->next;
	(*count)++;
	return (1);
}
char	**convert_l_env_to_char_env(t_env **env)
{
	char	**result;
	int		size_y;
	int		count;

	size_y = 0;
	count = 0;
	while ((*env)->previous != NULL)
		(*env) = (*env)->previous;
	while ((*env)->next != NULL)
	{
		(*env) = (*env)->next;
		size_y++;
	}
	while ((*env)->previous != NULL)
		(*env) = (*env)->previous;
	result = malloc(sizeof(char *) * (size_y + 1));
	if (!result)
		return (NULL);
	result[size_y] = NULL;
	while ((*env)->next != NULL)
		convert_l_env_to_char_env_utils(env, result, &count);
	return (result);
}

// void	launch_child_pipeline(t_cmd *cmd, t_env **envp)
// {

// }
