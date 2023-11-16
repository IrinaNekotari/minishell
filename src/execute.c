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

int	chain_as_equals(t_cmd **cmd, char *cmp)
{
	while ((*cmd)->tokens->str)
	{
		ft_printf("Comparing %s and %s\n", (*cmd)->tokens->str, cmp);
		if (ft_equals((*cmd)->tokens->str, cmp))
		{
			if (!(*cmd)->tokens->quote)
			{
				(*cmd)->tokens = (*cmd)->tokens->next;
				return (1);
			}
		}
		(*cmd)->tokens = (*cmd)->tokens->next;
	}
	rollback_tokens(cmd);
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
	rollback_cmd(&cmd);
	while (cmd)
	{
		if (ft_equals(cmd->tokens->str, "pwd"))
			ft_pwd(cmd, env);
		else if (ft_equals(cmd->tokens->str, "env"))
			ft_env(cmd, env);
		else if (ft_equals(cmd->tokens->str, "exit"))
			ft_exit(cmd, env);
		else
		{
			debug_show_all(cmd);
			execute_general(cmd, env);
		}
		cmd = cmd->pipe;
	}
}

void	execute(t_cmd *cmd, char **env)
{
	get_orders(cmd, env);
}
