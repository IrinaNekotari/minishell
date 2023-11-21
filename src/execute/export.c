/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 09:43:28 by nmascrie          #+#    #+#             */
/*   Updated: 2023/11/21 09:43:30 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_input(char *input)
{
	int	i;
	
	i = 0;
	if(ft_isdigit(input[i]) == 1)
		return (0);
	i++;
	while (input[i] && input[i] != '=')
	{
		if(ft_isalnum(input[i]) == 0)
			return (-1);
		i++;
	}
	return (1);
}

void	ft_export(t_cmd *cmd, t_env **env)
{
	char	*name;
	char	*value;
	
	if (!cmd->tokens->next->str)
	{
		print_sorted_env((*env));
		return ;
	}
	cmd->tokens = cmd->tokens->next;
	while (cmd->tokens->str)
	{
		if (!is_valid_input(cmd->tokens->str))
		{
			ft_printf("`%s': not a valid identifier\n", cmd->tokens->str);
			return ;
		}
		generate_env(cmd->tokens->str, &name, &value);
		add_to_env(env, name, value);
		cmd->tokens = cmd->tokens->next;
	}
}
