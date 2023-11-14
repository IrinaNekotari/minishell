/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 08:47:07 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/24 08:47:08 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_word *w)
{
	if (w->str)
		free(w->str);
	if (w->next)
		free_token(w->next);
	free(w);
}

void	free_io(t_io *io)
{
	if (io->file)
		free(io->file);
	if (io->next)
		free_io(io->next);
	free(io);
}

void	free_command(t_cmd *c)
{
	rollback_io(&c);
	rollback_tokens(&c);
	if (c->tokens)
		free_token(c->tokens);
	if (c->input)
		free_io(c->input);
	if (c->output)
		free_io(c->output);
	if (c->pipe)
		free_command(c->pipe);
	free(c);
}

void	free_liste(char **lst)
{
	int	i;

	i = 0;
	while (lst[i])
	{
		free(lst[i]);
		i++;
	}
	free(lst);
}
