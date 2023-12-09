/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_single.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:37:42 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/03 08:37:43 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_chevrons(t_cmd **cmd, char *s, int *i)
{
	char	*t;

	t = ft_calloc(3, sizeof(char));
	t[0] = s[*i];
	(*i) += 1;
	if (t[0] == '<' && s[*i] == '>')
	{
		t[1] = '>';
		(*i) += 1;
	}
	else if (s[*i] == t[0])
	{
		t[1] = s[*i];
		(*i) += 1;
	}
	add_word(cmd, t, 0, s[*i]);
	free(t);
}

void	part1(int *i, char *s, t_cmd **cmd, char *quote)
{
	char	*temp;

	if ((*i) == 0 || s[(*i) - 1] != '\\')
	{
		(*quote) = s[(*i)];
		(*i) += 1;
		temp = get_next_word(s, i, (*quote));
		add_word(cmd, temp, (*quote), s[*i + 1]);
		free(temp);
	}
}

void	part2(int *i, char *s, char *quote)
{
	if ((*i) == 0 || s[(*i) - 1] != '\\')
	{
		(*quote) = 0;
		(*i) += 1;
	}
}

void	part3(int *i, char *s, char *quote, t_cmd **cmd)
{
	char	*temp;

	if ((s[(*i)] == '>' || s[(*i)] == '<') && (*quote) == 0
		&& ((*i) == 0 || s[(*i) - 1] != '\\'))
		handle_chevrons(cmd, s, i);
	else
	{
		temp = get_next_word(s, i, 0);
		add_word(cmd, temp, (*quote), s[*i]);
		free(temp);
	}
}

void	parse_single(char *s, t_cmd **cmd)
{
	char		quote;
	int			i;

	i = 0;
	quote = 0;
	ft_printf("GOT #%s\n", s);
	while (s[i])
	{
		if (s[i] == '\\' && (i == 0 || s[i - 1] != '\\'))
			i++;
		if ((s[i] == '"' || s[i] == '\'') && quote == 0)
			part1(&i, s, cmd, &quote);
		if ((s[i] == '"' || s[i] == '\'') && quote != 0)
			part2(&i, s, &quote);
		if (!s[i])
			break ;
		while (s[i] && is_whitespace(s[i]))
			i++;
		part3(&i, s, &quote, cmd);
		while (s[i] && is_whitespace(s[i]))
			i++;
	}
	add_word(cmd, NULL, 0, 0);
	rollback_tokens(cmd);
}
