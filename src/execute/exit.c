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

/*static int	ft_isnum(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	if (str[0] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}*/

void	ft_exit(t_cmd *cmd, t_main *main)
{
	(void)cmd;
	(void)main;
	g_received_signal = -3;
	/*if (cmd->tokens->next->str && !ft_isnum(cmd->tokens->next->str))
	{
		ft_printf("minishell: exit: ");
		ft_printf("%s", cmd->tokens->next->str);
		ft_printf(": numeric argument required\n");
		exit(0);
	}
	else if (cmd->tokens->next->str)
	{
		if (cmd->tokens->next->next->str)
			ft_printf("minishell: exit: too many arguments\n");
		else
			exit(ft_atoi(cmd->tokens->next->str));
	}
	else
		exit(0);*/
}
