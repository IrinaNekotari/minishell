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

static void	add_text_pipe(char **s)
{
	char	*str;

	super_concat(s, " ");
	str = readline("> ");
	while (str && emptiness(str, 0))
	{
		free(str);
		str = readline("> ");
	}
	if (!str)
	{
		free((*s));
		(*s) = NULL;
		return ;
	}
	super_concat(s, str);
	free(str);
}

void	checks2(int i, int u, char *s)
{
	if (s[u + i + 1] == '|' && emptiness(s, u + i + 2))
	{
		g_received_signal = IGNORE_PIPES;
		add_text_pipe(&s);
	}
	if (emptiness(s, u + i + 1))
		add_text_pipe(&s);
}
