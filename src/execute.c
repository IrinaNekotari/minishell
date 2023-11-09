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
	t_word	*bck;

	bck = cmd->tokens;
	while (bck)
	{
		ft_printf("Comparing %s and %s\n", bck->str, cmp);
		if (ft_equals(bck->str, cmp))
		{
			if (!bck->quote)
			{
				bck = cmd->tokens;
				return (1);
			}
		}
		bck = bck->next;
	}
	bck = cmd->tokens;
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
	else if (ft_equals(cmd->tokens->str, "exit"))
		ft_exit(cmd, env);
	else
	{
		debug_show_all(cmd);
		execute_general(cmd, env);
	}
}

void	execute(t_cmd *cmd, char **env)
{
	get_orders(cmd, env);
}
