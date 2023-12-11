/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:36:26 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/03 08:36:27 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

static void	if_check_chevrons(t_cmd *cmd, t_main **main)
{
	if (check_chevrons(&cmd))
	{
		rollback_cmd(&cmd);
		generate_variables(&cmd, main);
		rollback_cmd(&cmd);
		generate_io(&cmd);
		rollback_cmd(&cmd);
		execute(cmd, main);
	}
}

void	parse(char *s, t_main **main)
{
	t_cmd	*cmd;
	int		i;

	cmd = NULL;
	(*main)->pipe_liste = (char **) ft_calloc(count_occur(s, '|') + 1,
			sizeof(int) * 100);
	(*main)->pipe_liste = counter_split(s, (*main)->pipe_liste);
	i = 0;
	while ((*main)->pipe_liste[i])
	{
		add_command(&cmd, (*main)->pipe_liste[i]);
		if (g_received_signal == IGNORE_PIPES)
			break ;
		i++;
	}
	if_check_chevrons(cmd, main);
	free_liste((*main)->pipe_liste);
	rollback_cmd(&cmd);
	free_command(cmd);
}

void	iterate(char *s, t_main *main)
{
	int		i;

	add_history(s);
	i = 0;
	main->iterate_liste = (char **) ft_calloc(count_occur(s, ';') + 2,
			sizeof(int) * 100);
	main->iterate_liste = split_semicolon(s, main->iterate_liste);
	while (main->iterate_liste[i])
	{
		parse(main->iterate_liste[i], &main);
		i++;
	}
	free_liste(main->iterate_liste);
}
