#include "minishell.h"

extern int	g_received_signal;

void	ft_exit(t_cmd *cmd, char **env)
{
	(void)cmd;
	(void)env;
	g_received_signal = -3;
}
