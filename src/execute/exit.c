#include "minishell.h"

extern int	g_received_signal;

void	ft_exit(t_cmd *cmd, t_main *main)
{
	(void)cmd;
	(void)main;
	g_received_signal = -3;
}
