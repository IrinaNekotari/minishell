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

int	count_occur(char *str, char c)
{
	int	i;
	int	count;
	
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			if ((i >= 1 && str[i - 1] != '\\') || i == 0)
				count++;
		}
		i++;
	}
	return (count);
}

void	init_command(t_cmd **cmd, char *line)
{
	(*cmd) = ft_calloc(1, sizeof(t_cmd));
	(*cmd)->pipe = NULL;
	(*cmd)->previous = NULL;
	parse_single(line, cmd);
}

void	add_command(t_cmd **cmd, char *line)
{
	if (!(*cmd))
		init_command(cmd, line);
	else
	{
		(*cmd)->pipe = ft_calloc(1, sizeof(t_cmd));
		(*cmd)->pipe->previous = (*cmd);
		(*cmd) = (*cmd)->pipe;
		(*cmd)->pipe = NULL;
		parse_single(line, cmd);
		//ft_printf("Found %s\n", (*cmd)->tokens->str);
	}
}

void	parse(char *s, t_main **main)
{
	t_cmd	*cmd;
	char	**t;
	int	i;

	cmd = NULL;
	t = (char **) ft_calloc(count_occur(s, '|') + 1, sizeof(int) * 100);
	t = counter_split(s, t);
	i = 0;
	while (t[i])
	{
		add_command(&cmd, t[i]);
		i++;
	}
	if (check_chevrons(&cmd))
	{
		rollback_cmd(&cmd);
		generate_variables(&cmd, main);
		rollback_cmd(&cmd);
		generate_io(&cmd);
		rollback_cmd(&cmd);
		execute(cmd, main);
	}
	free_liste(t);
	free_command(cmd);
}

void	parse_with_pipes(char **t, t_cmd **c)
{
	int		i;
	i = 0;
	while (t[i])
	{
		parse_single(t[i], c);
		rollback_io(c);
		rollback_tokens(c);
		if (t[i + 1])
		{
			(*c)->pipe = ft_calloc(1, sizeof(t_cmd));
			(*c)->pipe->previous = (*c);
			(*c) = (*c)->pipe;
		}
		else
			(*c)->pipe = NULL;
		i++;
	}
	rollback_cmd(c);
}

void	iterate(char *s, t_main *main)
{
	char	**lst;
	int		i;

	i = 0;

	//TODO : Chercher la bonne taille
	lst = (char **) ft_calloc(count_occur(s, ';') + 1, sizeof(int) * 100);
	lst = split_semicolon(s, lst);
	log_input(s);
	while (lst[i])
	{
		parse(lst[i], &main);
		i++;
	}
	free_liste(lst);
}
