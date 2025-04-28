#include <minishell.h>

t_lexer *start_lexer(char *cmd_line)
{
	t_lexer lexer;
	char **cmd;
	int		i;

	cmd = NULL;
	cmd = ft_split(cmd_line);
	i = 0;
	while (cmd[i])
	{

		i++;
	}
