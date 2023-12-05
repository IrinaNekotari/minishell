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
	char	**t;
	int		i;

	cmd = NULL;
	t = (char **) ft_calloc(count_occur(s, '|') + 1, sizeof(int) * 100);
	t = counter_split(s, t);
	i = 0;
	while (t[i])
	{
		add_command(&cmd, t[i]);
		if (g_received_signal == IGNORE_PIPES)
			break ;
		i++;
	}
	if_check_chevrons(cmd, main);
	free_liste(t);
	free_command(cmd);
}

void	iterate(char *s, t_main *main)
{
	char	**lst;
	int		i;

	i = 0;
	lst = (char **) ft_calloc(count_occur(s, ';') + 1, sizeof(int) * 100);
	//TODO : Chercher la bonne taille
	lst = (char **) ft_calloc(count_occur(s, ';') + 2, sizeof(int) * 100);
	lst = split_semicolon(s, lst);
	while (lst[i])
	{
		parse(lst[i], &main);
		i++;
	}
	free_liste(lst);
}
