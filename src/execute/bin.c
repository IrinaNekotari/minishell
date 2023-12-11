/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 14:22:12 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/30 14:22:13 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

static int	el_check(int exit_val)
{
	if (g_received_signal == SIGNAL_QUIT)
		return (1);
	if (g_received_signal == SIGNAL_ABORT)
		return (1);
	if (exit_val == SIGNAL_ABORT)
		return (1);
	return (0);
}

int	ft_exec(t_cmd *cmd, t_main **main)
{
	int	pid;
	int	i;
	int	exit_val;

	if (!cmd->pipe)
		(*main)->state = LAST_PIPE;
	i = pipe((*main)->pipes);
	exit_val = 0;
	if (cmd->pipe && i == -1)
		return (error_print
			(CRITICAL, "An error has occured while piping !", NULL));
	pid = fork();
	if (pid < 0)
		return (error_print
			(CRITICAL, "An error has occured while forking !", NULL));
	if (pid == 0)
		fork_core(cmd, main);
	else
		exit_val = fork_returns(cmd, main, pid);
	if (!cmd->pipe || el_check(exit_val) || exit_val != 0)
		return (exit_val);
	cmd = cmd->pipe;
	(*main)->state = IN_PIPE;
	ft_exec(cmd, main);
	return (exit_val);
}
