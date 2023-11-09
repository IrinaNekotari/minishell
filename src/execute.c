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
