/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   countersplit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:27:03 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/24 16:27:04 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_trips(t_triplet *t, int *a, char *b)
{
	t->i = 0;
	t->j = 0;
	t->k = 0;
	(*a) = 0;
	(*b) = 0;
}

void	check_semi(t_triplet *t, char *s, int *quoted, char ***to_ret)
{
	if (s[t->i] == ';' && (*quoted) == 0)
	{
		if ((s[t->i - 1] != '\\') || t->i == 0)
		{
			t->j = 0;
			t->k += 1;
			t->i += 1;
			(*to_ret)[t->k] = ft_calloc(ft_strlen(s) + 1, sizeof(char));
		}
	}
}

void	check_pippe(t_triplet *t, char *s, int *quoted, char ***to_ret)
{
	if (s[t->i] == '|' && (*quoted) == 0)
	{
		if ((s[t->i - 1] != '\\') || t->i == 0)
		{
			t->j = 0;
			t->k += 1;
			t->i += 1;
			(*to_ret)[t->k] = ft_calloc(ft_strlen(s) + 1, sizeof(char));
		}
	}
}

//TODO : Norminer tout ca
char	**split_semicolon(char *s, char **to_ret)
{
	t_triplet	trip;
	int			quoted;
	char		quote;

	to_ret[0] = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	init_trips(&trip, &quoted, &quote);
	while (s[trip.i])
	{
		part1_counter(&trip.i, s, &quote, &quoted);
		check_semi(&trip, s, &quoted, &to_ret);
		if (!s[trip.i])
			break ;
		if (is_whitespace(s[trip.i]) && quoted == 0)
		{
			to_ret[trip.k][trip.j++] = s[trip.i++];
			while (is_whitespace(s[trip.i]))
				trip.i++;
		}
		else
			to_ret[trip.k][trip.j++] = s[trip.i++];
	}
	to_ret[trip.k][trip.j] = 0;
	return (to_ret);
}

char	**counter_split(char *s, char **to_ret)
{
	t_triplet	trip;
	int			quoted;
	char		quote;

	to_ret[0] = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	init_trips(&trip, &quoted, &quote);
	while (s[trip.i])
	{
		part1_counter(&(trip.i), s, &quote, &quoted);
		check_pippe(&trip, s, &quoted, &to_ret);
		if (is_whitespace(s[trip.i]) && quoted == 0)
		{
			to_ret[trip.k][trip.j++] = s[trip.i++];
			while (is_whitespace(s[trip.i]))
				trip.i++;
		}
		else
			to_ret[trip.k][trip.j++] = s[trip.i++];
	}
	to_ret[trip.k][trip.j] = 0;
	return (to_ret);
}
