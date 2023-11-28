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
	(void)cmd;
	(void)main;
	g_received_signal = -3;
}
