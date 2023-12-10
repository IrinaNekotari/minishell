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
	if (cmd->tokens->next->str && cmd->tokens->next->next->str)
		error_print(ERROR, "Too many arguments", NULL);
	else if (cmd->tokens->next->str)
	{
		main->code_exit = ft_atoi(cmd->tokens->next->str);
		g_received_signal = SIGNAL_QUIT;
	}
	else
		g_received_signal = SIGNAL_QUIT;
}
