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

	(*main)->state = FIRST_PIPE;
	(*main)->mode = 0;
	if (is_system(cmd) && !cmd->pipe)
		(*main)->last = exec_builtin(cmd, main);
	else
	{
		pid = fork();
		if (pid < 0)
		{
			error_print(CRITICAL, "An error has occured while forking !", NULL);
			(*main)->last = -1;
			return ;
		}
		else if (!pid)
			ft_exec(cmd, main);
		else
			waitpid(pid, &((*main)->last), 0);
	}
	//Mais la, c'est plus le même code ?????????
	ft_putstr_fd(ft_itoa((*main)->last), 2);
}
