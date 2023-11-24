#include "minishell.h"

extern int	g_received_signal;

void	ft_echo(t_cmd *cmd, t_env *env)
{
	char	*str;

	(void)env;
	str = ft_calloc(1, sizeof(char));
	cmd->tokens = cmd->tokens->next;
	if (ft_equals(cmd->tokens->str, "-n"))
	{
		g_received_signal = IGNORE_NEW_LINE;
		cmd->tokens = cmd->tokens->next;
	}
	while (cmd->tokens->str)
	{
		super_concat(&str, cmd->tokens->str);
		if (cmd->tokens->next->str)
			super_concat(&str, " ");
		cmd->tokens = cmd->tokens->next;
	}
	print_io(cmd, str);
	free(str);
}
