/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 13:11:16 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/20 13:11:17 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset(t_cmd *cmd, t_env *env)
{
	if (get_env(env, env->name) == NULL)
	{
		return ;
	}
	else
	{
		del_from_env(&env, env->name);
	}
}
