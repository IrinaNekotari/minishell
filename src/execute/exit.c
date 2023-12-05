/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:20:04 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/28 14:20:05 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

void	ft_exit(t_cmd *cmd, t_main *main)
{
	(void)main;
	if (cmd->tokens->next->str)
		ft_printf("minishell: exit: too many arguments\n");
	else
		g_received_signal = SIGNAL_QUIT;
}
