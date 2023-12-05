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

char	**split_semi(char *s, char **to_ret)
{
	int		i;
	int		quoted;
	char	quote;

	i = 0;
	quote = 0;
	while(s[i])
	{
		if ((s[i] == '\"' || s[i] == '\'') && quoted == 0)
		{
			quoted = 1;
			quote = s[i];
		}
	}
	return (to_ret);
}

//TODO : Norminer tout ca
char	**split_semicolon(char *s, char **to_ret)
{
	int		i;
	int		j;
	int		k;
	int		quoted;
	char	quote;

	to_ret[0] = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	i = 0;
	j = 0;
	quoted = 0;
	quote = 0;
	k = 0;
	while (s[i])
	{
		if ((s[i] == '\"' || s[i] == '\'') && quoted == 0)
		{
			if ((s[i - 1] != '\\') || i == 0)
			{
				quoted = 1;
				quote = s[i];
			}
		}
		else if (s[i] == quote && quoted == 1 && ((s[i - 1] != '\\') || i == 0))
			quoted = 0;
		if (s[i] == ';' && quoted == 0)
		{
			if ((s[i - 1] != '\\') || i == 0)
			{
				j = 0;
				k++;
				i++;
				to_ret[k] = ft_calloc(ft_strlen(s) + 1, sizeof(char));
			}
		}
		if (is_whitespace(s[i]) && quoted == 0)
		{
			to_ret[k][j++] = s[i++];
			while (is_whitespace(s[i]))
				i++;
		}
		else
			to_ret[k][j++] = s[i++];
	}
	to_ret[k][j] = 0;
	return (to_ret);
}

//TODO : Norminer tout ca
char	**counter_split(char *s, char **to_ret)
{
	int		i;
	int		j;
	int		k;
	int		quoted;
	char	quote;

	to_ret[0] = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	i = 0;
	j = 0;
	quoted = 0;
	k = 0;
	quote = 0;
	while (s[i])
	{
		if ((s[i] == '\"' || s[i] == '\'') && quoted == 0)
		{
			if ((s[i - 1] != '\\') || i == 0)
			{
				quoted = 1;
				quote = s[i];
			}
		}
		else if (s[i] == quote && quoted == 1 && ((s[i - 1] != '\\') || i == 0))
			quoted = 0;
		if (s[i] == '|' && quoted == 0)
		{
			if ((s[i - 1] != '\\') || i == 0)
			{
				j = 0;
				k++;
				to_ret[k] = ft_calloc(ft_strlen(s) + 1, sizeof(char));
				i++;
				while (is_whitespace(s[i]))
					i++;
			}
		}
		if (is_whitespace(s[i]) && quoted == 0)
		{
			to_ret[k][j++] = s[i++];
			while (is_whitespace(s[i]))
				i++;
			if (!s[i])
			{
				to_ret[k][j - 1] = 0;
				break ;
			}
		}
		else
			to_ret[k][j++] = s[i++];
	}
	to_ret[k][j] = 0;
	return (to_ret);
}
