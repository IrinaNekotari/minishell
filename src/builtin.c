#include "minishell.h"

int	is_system(t_cmd *cmd)
{
	if (ft_equals(cmd->tokens->str, "exit"))
		return (1);
	else if (ft_equals(cmd->tokens->str, "cd"))
		return (1);
	else if (ft_equals(cmd->tokens->str, "unset"))
		return (1);
	else if (ft_equals(cmd->tokens->str, "export") && cmd->tokens->next->str)
		return (1);
	return (0);
}

int	is_builtin(char *str)
{
	if (ft_equals(str, "pwd"))
		return (1);
	else if (ft_equals(str, "env"))
		return (1);
	else if (ft_equals(str, "exit"))
		return (1);
	else if (ft_equals(str, "echo"))
		return (1);
	else if (ft_equals(str, "export"))
		return (1);
	else if (ft_equals(str, "unset"))
		return (1);
	else if (ft_equals(str, "cd"))
		return (1);
	else
		return (0);
}

int	exec_builtin(t_cmd *cmd, t_main **main)
{
	g_received_signal = IGNORE_NEW_LINE;
	(*main)->mode = 1;
	if (ft_equals(cmd->tokens->str, "pwd"))
		ft_pwd(cmd, (*main));
	else if (ft_equals(cmd->tokens->str, "env"))
		ft_env(cmd, (*main));
	else if (ft_equals(cmd->tokens->str, "exit"))
		ft_exit(cmd, (*main));
	else if (ft_equals(cmd->tokens->str, "echo"))
		ft_echo(cmd, (*main));
	else if (ft_equals(cmd->tokens->str, "export"))
		ft_export(cmd, main);
	else if (ft_equals(cmd->tokens->str, "unset"))
		ft_unset(cmd, main);
	else if (ft_equals(cmd->tokens->str, "cd"))
		ft_cd(cmd, main);
	if (g_received_signal == SIGNAL_QUIT || g_received_signal == SIGNAL_ABORT)
		return (-1);
	return (0);
}
