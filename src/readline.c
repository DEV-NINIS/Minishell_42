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
			lexer = handle_line(rl);
			while (lexer->previous != NULL)
				lexer = lexer->previous;
			while (lexer->next != NULL)
			{
				printf("%s\n", lexer->content);
				lexer = lexer->next;
			}
			printf("%s\n", lexer->content);
		}
		printf("%s\n", rl);
		add_history(rl);
	}
	free_env(&env_result);
	free(rl);
	return (0);
}
