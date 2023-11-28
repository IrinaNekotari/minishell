/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _io_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 13:43:51 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/28 13:44:06 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	search_for_double(t_cmd *cmd)
{
	int	ret;

	ret = 0;
	while (cmd->input->file)
	{
		if (cmd->input->io == DOUBLE_INPUT)
		{
			ret = 1;
			break ;
		}
		cmd->input = cmd->input->next;
	}
	while (cmd->input->previous)
		cmd->input = cmd->input->previous;
	return (ret);
}

static void	loop_rl(t_cmd *cmd, char *sortie)
{
	sortie = readline("> ");
	while (!ft_equals(sortie, cmd->input->file))
	{
		free(sortie);
		sortie = readline("> ");
	}
	free(sortie);
}

static void	handle_double_input(t_cmd *cmd)
{
	char	*sortie;

	sortie = NULL;
	while (cmd->input->file)
		cmd->input = cmd->input->next;
	while (cmd->input)
	{
		if (cmd->input->io == DOUBLE_INPUT)
			loop_rl(cmd, sortie);
		if (!cmd->input->previous)
			break ;
		cmd->input = cmd->input->previous;
	}
	cmd->input = cmd->input->next;
	while (cmd->input->file)
	{
		if (cmd->input->io == DOUBLE_INPUT)
			loop_rl(cmd, sortie);
		cmd->input = cmd->input->next;
	}
	while (cmd->input->previous)
		cmd->input = cmd->input->previous;
}

int	handle_input(t_cmd *cmd)
{
	int	fd;

	if (search_for_double(cmd))
		handle_double_input(cmd);
	while (cmd->input->file)
	{
		fd = open(cmd->input->file, O_RDONLY);
		if (cmd->input->io == SINGLE_INPUT && fd == -1)
		{
			error_print(ERROR, "Not a file or directory",
				cmd->input->file);
			return (0);
		}
		if (fd >= 0)
			close(fd);
		cmd->input = cmd->input->next;
	}
	return (1);
}
