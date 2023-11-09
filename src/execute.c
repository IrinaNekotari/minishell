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

int	chain_as_equals(t_cmd *cmd, char *cmp)
{
	t_cmd	*bck;

	bck = cmd;
	while (bck->tokens)
	{
		if (ft_equals(bck->tokens->str, cmp))
		{
			if (!bck->tokens->quote)
				return (1);
		}
		if (!bck->tokens->next)
			return (0);
		bck->tokens = bck->tokens->next;
	}
	return (0);
}

int	ft_equals(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	if (s1[i] != s2[i])
		return (0);
	return (1);
}

void	execute_general(t_cmd *cmd, char **env)
{
	(void)cmd;
	(void)env;
}

void	get_orders(t_cmd *cmd, char **env)
{
	if (ft_equals(cmd->tokens->str, "pwd"))
		ft_pwd(cmd, env);
	else
		execute_general(cmd, env);
}

void	execute(t_cmd *cmd, char **env)
{
	debug_show_all(cmd);
	get_orders(cmd, env);
	return ;
}
