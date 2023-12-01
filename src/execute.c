/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:00:58 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/24 14:00:59 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

void	get_orders(t_cmd *cmd, t_main **main)
{
	while (cmd)
	{
		if (!cmd->pipe)
			(*main)->state = LAST_PIPE;
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
		else
			execute_general(cmd, (*main));
		if (g_received_signal == -3)
			break ;
		cmd = cmd->pipe;
		(*main)->state = IN_PIPE;
	}
}

void	execute(t_cmd *cmd, t_main **main)
{
	int	pid;
	int	ret;

	ret = 0;
	(*main)->state = FIRST_PIPE;
	(*main)->mode = 0;
	if (is_system(cmd))
		exec_builtin(cmd, main);
	else
	{
		pid = fork();
		if (!pid)
			ft_exec(cmd, main);
		waitpid(pid, &ret, 0);
	}
	if (ret == -3)
		return ;
	if (cmd->pipe)
		cmd = cmd->pipe;
	else
		return ;
	execute(cmd, main);
}
