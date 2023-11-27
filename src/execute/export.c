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

static char	*create_list(t_env *env)
{
	char *a;

	a = ft_calloc(1, sizeof(char));
	while (env)
	{
		super_concat(&a, "declare -x ");
		super_concat(&a, env->name);
		super_concat(&a, "=");
		super_concat(&a, env->value);
		if (env->next)
			super_concat(&a, "\n");
		env = env->next;
	}
	return (a);
}

void	ft_export(t_cmd *cmd, t_main *main)
{
	char	*vars;
	
	if (!cmd->tokens->next->str)
	{
		vars = create_list(main->env);
		print_io(cmd, vars, &main);
		free(vars);
	}
	else
	{
		vars = create_list(main->env);
		cmd->tokens = cmd->tokens->next;
		while (cmd->tokens->str)
		{
			if (!is_valid_input(cmd->tokens->str))
			{
				error_print(ERROR, "not a valid identifier",cmd->tokens->str);
				return ;
			}
			super_concat(&vars, "\n");
			super_concat(&vars, cmd->tokens->str);
			cmd->tokens = cmd->tokens->next;
		}
		print_io(cmd, vars, &main);
		free(vars);
	}
}
