/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 15:19:28 by mjuette           #+#    #+#             */
/*   Updated: 2023/12/04 15:19:31 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

static int	checks(int i, int u, char *s)
{
	if ((i == 0 && s[u + i + 1] == '|')
		|| (s[u + i + 1] == '|'
			&& s[u + i + 2] == '|' && s[u + i + 3] == '|'))
	{
		error_syntax(ERROR, "||");
		free(s);
		return (0);
	}
	if (i == 0 || (s[u + i + 1] == '|' && s[u + i + 2] == '|'))
	{
		error_syntax(ERROR, "|");
		free(s);
		return (0);
	}
	if (s[u + i + 1] == '|' && s[u + i + 2])
	{
		g_received_signal = IGNORE_PIPES;
		i++;
	}
	return (1);
}

static int	checks3(int i, int u, char *s)
{
	if (!checks(i, u, s))
		return (0);
	if (!checks2(i, u, s))
		return (0);
	if (!s)
		return (0);
	return (1);
}

static void	init_vars(int *a, int *b, int *c, char *d)
{
	(*a) = 0;
	(*b) = 0;
	(*c) = 0;
	(*d) = 0;
}

char	*check_pipes(char *s)
{
	int		quote;
	int		i;
	int		u;
	char	quoted;

	init_vars(&i, &u, &quote, &quoted);
	if (!s)
		return (NULL);
	while (is_whitespace(s[u]))
		u++;
	while (s[u + i])
	{
		if ((s[u + i] == '"' || s[u + i] == '\'') && quote == 0)
		{
			quote = 1;
			quoted = s[u + i];
		}
		else if (s[u + i] == quoted && quote == 1)
			quote = 0;
		else if (s[u + i] == '|' && quote == 0)
			if (!checks3(i, u, s))
				return (NULL);
		i++;
	}
	return (s);
}
