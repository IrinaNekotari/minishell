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

void	execute(t_cmd *cmd, t_main **main)
{
	int	pid;
	int	ret;

	ret = 0;
	(*main)->state = FIRST_PIPE;
	(*main)->mode = 0;
	if (is_system(cmd) && !cmd->pipe && !cmd->previous)
		exec_builtin(cmd, main);
	else
	{
		pid = fork();
		if (!pid)
			ft_exec(cmd, main);
		waitpid(pid, &ret, 0);
	}
	(*main)->last = ret;
	if (ret == -3)
		return ;
	if (cmd->pipe)
		cmd = cmd->pipe;
	else
		return ;
	execute(cmd, main);
}
