/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:00:50 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/24 14:00:51 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_show_command(t_word *t)
{
	int		i;
	t_word	*bck;

	i = 0;
	bck = t;
	while (bck)
	{
		if (bck->str)
		{
			ft_printf("💀️ Token %d = %s", i, bck->str);
			if (bck->quote)
				ft_printf(", Quoted with %c", bck->quote);
			ft_printf("\n");
		}
		if (bck->next)
			bck = bck->next;
		else
			break ;
		i++;
	}
}

void	debug_show_input(t_io *io)
{
	t_io	*bck;

	bck = io;
	ft_printf("Found inputs for the command\n");
	while (bck)
	{
		if (bck->file)
		{
			ft_printf("🤖️ Input %s", bck->file);
			if (bck->io == SINGLE_INPUT)
				ft_printf(", using <\n");
			else
				ft_printf(", using <<\n");
		}
		if (bck->next)
			bck = bck->next;
		else
			break ;
	}
}

void	debug_show_output(t_io *io)
{
	t_io	*bck;

	bck = io;
	ft_printf("Found outputs for the command\n");
	while (bck)
	{
		if (bck->file)
		{
			ft_printf("👺️ Output %s", bck->file);
			if (bck->io == SINGLE_OUTPUT)
				ft_printf(", using >\n");
			else
				ft_printf(", using >>\n");
		}
		if (bck->next)
			bck = bck->next;
		else
			break ;
	}
}

void	debug_show_all(t_cmd *c)
{
	t_cmd	*bck;

	bck = c;
	while (bck)
	{
		if (bck->tokens)
			debug_show_command(bck->tokens);
		if (bck->pipe)
			ft_printf("Found a \x1b[36mPipe | \x1b[37m!\n");
		if (bck->pipe)
			bck = bck->pipe;
		else
			break ;
	}
	if (c->input->file)
		debug_show_input(c->input);
	if (c->output->file)
		debug_show_output(c->output);
}
