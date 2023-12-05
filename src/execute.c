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
	int	ret;

	(*main)->state = FIRST_PIPE;
	(*main)->mode = 0;
	ret = 0;
	if (is_system(cmd) && !cmd->pipe)
		(*main)->last = exec_builtin(cmd, main);
	else
	{
		ret = ft_exec(cmd, main);
		((*main)->last) = ret;
	}
}
