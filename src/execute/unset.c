/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 09:43:34 by nmascrie          #+#    #+#             */
/*   Updated: 2023/11/21 09:43:36 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_system_env(char *s)
{
	if (ft_equals(s, "PWD"))
		return (1);
	if (ft_equals(s, "PATH"))
		return (1);
	return (0);
}

static int	is_valid_input(char *input)
{
	int	i;

	i = 0;
	if (ft_isdigit(input[i]) == 1)
		return (0);
	i++;
	while (input[i] && input[i] != '=')
	{
		if (ft_isalnum(input[i]) == 0)
			return (-1);
		i++;
	}
	return (1);
}

void	ft_unset(t_cmd *cmd, t_main **main)
{
	int	boolean;

	boolean = 0;
	if (!cmd->tokens->next->str)
	{
		error_print(ERROR, "not enough arguments !", NULL);
		g_received_signal = SIGNAL_ABORT;
		return ;
	}
	if (is_system_env(cmd->tokens->next->str))
		error_env();
	cmd->tokens = cmd->tokens->next;
	while (cmd->tokens->str)
	{
		if (!is_valid_input(cmd->tokens->str) && boolean == 0)
		{
			error_print(ERROR, "not a valid identifier",
				cmd->tokens->str);
			boolean = 1;
		}
		del_from_env(&((*main)->env), cmd->tokens->str);
		cmd->tokens = cmd->tokens->next;
	}
}
