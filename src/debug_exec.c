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
}
