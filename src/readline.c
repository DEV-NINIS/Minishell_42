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

	env_result = NULL;
	if (!argc || !argv)
		return (0);
	env_result = init_env_list(env);
	while (env_result->next != NULL)
	{
		printf("key -> %s ; value -> %s\n", env_result->current_key, env_result->current_value);
		env_result = env_result->next;
	}
	printf("key -> %s ; value -> %s\n", env_result->current_key, env_result->current_value);

	
	signal(SIGINT, handle_signal_interrupt);
	rl = NULL;	
	while ((rl = getmyline()) && ft_strcmp(rl, "exit") != 0)
	{
		if (rl[0] == 'a')
			builtin_pwd();
		printf("%s\n", rl);
		add_history(rl);
	}
	free_env(&env_result);
	free(rl);
	return (0);
}
