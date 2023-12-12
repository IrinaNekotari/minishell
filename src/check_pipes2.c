/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipes2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:04:53 by nmascrie          #+#    #+#             */
/*   Updated: 2023/12/05 14:04:54 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

static int	emptiness(char *s, int i)
{
	while (is_whitespace(s[i]))
		i++;
	if (s[i])
		return (0);
	return (1);
}

int	checks2(int i, int u, char *s)
{
	if (s[u + i + 1] == '|' && emptiness(s, u + i + 2)
		&& (i == 0 || s[i - 1] != '\\'))
	{
		g_received_signal = IGNORE_PIPES;
		error_syntax(ERROR, "||");
		free(s);
		return (0);
	}
	if (emptiness(s, u + i + 1) && (i == 0 || s[i - 1] != '\\'))
	{
		error_syntax(ERROR, "|");
		free(s);
		return (0);
	}
	return (1);
}
