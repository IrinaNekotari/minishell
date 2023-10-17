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

void	debug_show_command(t_word *t)
{
	int	i;
	t_word		*bck;

	i = 0;
	bck = t;
	while (bck)
	{
		ft_printf("Token %d = %s\n", i, bck->str);
		if (bck->next)
			bck = bck->next;
		else
			break ;
		i++;
	}
}

void	execute(t_word *cmd, char **env)
{
	debug_show_command(cmd);
	(void) env;
	return ;
}

void	parse(char *s, char **env)
{
	t_word	*command;

	command = malloc(sizeof(t_word));
	if (ft_strchr(s, '|'))
		parse_with_pipes(ft_split(s, '|'), command);
	else
		parse_single(s, command);
	execute(command, env);
}

void	iterate(char *s, char **env)
{
	char **lst;
	int	i;
	
	i = 0;
	lst = ft_split(s, ';');
	while (lst[i])
	{
		ft_printf("DEBUG : Found line\x1b[32m %s\x1b[37m\n", lst[i]);
		parse(lst[i], env);
		i++;
	}
}
