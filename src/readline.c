#include "../inc/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	while (s1[i] == s2[i] && s1[i])
		i++;
	return ((int)s1[i] - (int)s2[i]);
}

char *getmyline(void)
{
	char *buf = NULL;

	if (buf)
	{
		free(buf);
		buf = NULL;
	}
	buf = readline("Minishell > ");
	return (buf);
}

int	main(int argc, char **argv, char **env)
{
	char *rl;
	t_env	*env_result;
	t_lexer	*lexer;

	env_result = NULL;
	if (!argc || !argv)
		return (0);
	env_result = init_env_list(env);

	lexer = NULL;
	signal(SIGINT, handle_signal_interrupt);
	rl = NULL;
	while ((rl = getmyline()) && ft_strcmp(rl, "exit") != 0)
	{
		if (ft_strlen(rl) >= 10 && (lexer || !lexer))
		{
			
			t_cmd *cmd1 = malloc(sizeof(t_cmd));
			t_cmd *cmd2 = malloc(sizeof(t_cmd));

			// Commande 1 : ls -l
			cmd1->args = malloc(sizeof(char *) * 3);
			cmd1->args[0] = strdup("wc");
			cmd1->args[1] = ft_strdup("-l");
			cmd1->args[2] = NULL;
			cmd1->infile = NULL;
			cmd1->outfile = NULL;
			cmd1->append = 0;
			cmd1->heredoc = 1;
			cmd1->heredoc_delimiter = ft_strdup("anis");
			cmd1->next = cmd2;
			cmd1->previous = NULL;

			// // Commande 2 : grep txt
			cmd2->args = malloc(sizeof(char *) * 3);
			cmd2->args[0] = strdup("wc");
			cmd2->args[1] = ft_strdup("-l");
			cmd2->args[2] = NULL;
			cmd2->infile = NULL;
			cmd2->outfile = NULL;
			cmd2->append = 0;
			cmd2->heredoc = 1;
			cmd2->heredoc_delimiter = ft_strdup("anis");
			cmd2->next = NULL;
			cmd2->previous = cmd1;
			

			execute_pipeline(cmd1, &env_result);

			// lexer = handle_line(rl);
			// while (lexer->previous != NULL)
			// 	lexer = lexer->previous;
			// while (lexer->next != NULL)
			// {
			// 	printf("%s\n", lexer->content);
			// 	lexer = lexer->next;
			// }
			// printf("%s\n", lexer->content);
			// launch_child_process(add_path_build("git add -A", "/usr/bin/"), argv, env);
		}
		printf("%s\n", rl);
		add_history(rl);
		free(rl);
	}
	free_env(&env_result);
	return (0);
}
