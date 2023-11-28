/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   links_rollbacks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:28:03 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/28 14:28:05 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rollback_tokens(t_cmd **cmd)
{
	if (!(*cmd) || !(*cmd)->tokens)
		return ;
	while ((*cmd)->tokens->previous)
		(*cmd)->tokens = (*cmd)->tokens->previous;
}

void	rollback_env(t_env **env)
{
	if (!(*env))
		return ;
	while ((*env)->previous)
		(*env) = (*env)->previous;
}

void	rollback_io(t_cmd **cmd)
{
	if (!(*cmd) || !(*cmd)->input || !(*cmd)->output)
		return ;
	while ((*cmd)->input->previous)
		(*cmd)->input = (*cmd)->input->previous;
	while ((*cmd)->output->previous)
		(*cmd)->output = (*cmd)->output->previous;
}

void	rollback_cmd(t_cmd **cmd)
{
	if (!(*cmd) || !(*cmd)->previous)
		return ;
	while ((*cmd)->previous)
		(*cmd) = (*cmd)->previous;
}
