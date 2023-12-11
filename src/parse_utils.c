/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:13:38 by mjuette           #+#    #+#             */
/*   Updated: 2023/12/05 14:13:40 by mjuette          ###   ########.fr       */
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

int	delimm(char *str, int *i)
{
	if (is_whitespace(str[*i]))
		return (1);
	if (*i == 0 || str[(*i) - 1] != '\\')
	{
		if (is_delim(str[*i]))
			return (1);
	}
	return (0);
}

void	init_command(t_cmd **cmd, char *line)
{
	(*cmd) = ft_calloc(1, sizeof(t_cmd));
	(*cmd)->pipe = NULL;
	(*cmd)->previous = NULL;
	(*cmd)->pipes = ft_calloc(2, sizeof(int));
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
		(*cmd)->pipes = ft_calloc(2, sizeof(int));
		parse_single(line, cmd);
	}
}
