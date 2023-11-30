/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   depths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:16:56 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/28 14:16:58 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
*	Retourne la profondeur de l'input
*/
int	input_depth(t_cmd *cmd)
{
	int	depth;

	depth = 0;
	while (cmd->input->file)
	{
		depth++;
		cmd->input = cmd->input->next;
	}
	while (cmd->input->previous)
		cmd->input = cmd->input->previous;
	return (depth);
}

/**
*	Retourne la profondeur de l'output
*/
int	output_depth(t_cmd *cmd)
{
	int	depth;

	depth = 0;
	while (cmd->output->file)
	{
		depth++;
		cmd->output = cmd->output->next;
	}
	while (cmd->output->previous)
		cmd->output = cmd->output->previous;
	return (depth);
}

/**
*	Retourne la profondeur des tokens
*/
int	tokens_depth(t_cmd *cmd)
{
	int	depth;

	depth = 0;
	while (cmd->tokens->str)
	{
		depth++;
		cmd->tokens = cmd->tokens->next;
	}
	while (cmd->tokens->previous)
		cmd->tokens = cmd->tokens->previous;
	return (depth);
}

/**
*	Retourne la profondeur de l'env
*/
int	env_depth(t_env *env)
{
	int	depth;

	depth = 0;
	while (env)
	{
		depth++;
		if (env->next)
			env = env->next;
		else
			break ;
	}
	while (env->previous)
		env = env->previous;
	return (depth);
}
