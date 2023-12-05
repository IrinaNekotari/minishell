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

void	init_tokens(t_cmd **cmd, char *str, char quote)
{
	(*cmd)->tokens = ft_calloc(1, sizeof(t_word));
	(*cmd)->tokens->next = NULL;
	(*cmd)->tokens->previous = NULL;
	if (str)
		(*cmd)->tokens->str = ft_strdup(str);
	else
		(*cmd)->tokens->str = NULL;
	(*cmd)->tokens->quote = quote;
}

void	add_word(t_cmd **cmd, char *str, char quote)
{
	if (!(*cmd)->tokens)
		init_tokens(cmd, str, quote);
	else
	{
		(*cmd)->tokens->next = ft_calloc(1, sizeof(t_word));
		(*cmd)->tokens->next->previous = (*cmd)->tokens;
		(*cmd)->tokens = (*cmd)->tokens->next;
		if (str)
			(*cmd)->tokens->str = ft_strdup(str);
		else
			(*cmd)->tokens->str = NULL;
		(*cmd)->tokens->quote = quote;
		(*cmd)->tokens->next = NULL;
	}
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

char	*get_next_word(char *str, int *i, char delim)
{
	int		j;
	char	*ret;

	j = 0;
	ret = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	while (str[*i])
	{
		if (delim == 0 && delimm(str, i))
			break ;
		else if (str[*i] == delim)
			break ;
		ret[j] = str[*i];
		j++;
		(*i) += 1;
	}
	return (ret);
}

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
	add_word(cmd, t, 0);
	free(t);
}

void	parse_single(char *s, t_cmd **cmd)
{
	char		quote;
	char		*temp;
	int			i;

	i = 0;
	quote = 0;
	while (s[i])
	{
		if (s[i] == '\\' && (i == 0 || s[i - 1] != '\\'))
			i++;
		if ((s[i] == '"' || s[i] == '\'') && quote == 0)
		{
			if (i == 0 || s[i - 1] != '\\')
			{
				quote = s[i++];
				temp = get_next_word(s, &i, quote);
				add_word(cmd, temp, quote);
				free(temp);
			}
		}
		if ((s[i] == '"' || s[i] == '\'') && quote != 0)
		{
			if (i == 0 || s[i - 1] != '\\')
			{
				quote = 0;
				i++;
			}
		}
		if (!s[i])
			break ;
		while (s[i] && is_whitespace(s[i]))
			i++;
		if ((s[i] == '>' || s[i] == '<') && quote == 0 && (i == 0 || s[i - 1] != '\\'))
			handle_chevrons(cmd, s, &i);
		else
		{
			temp = get_next_word(s, &i, 0);
			add_word(cmd, temp, quote);
			free(temp);
		}
		while (s[i] && is_whitespace(s[i]))
			i++;
	}
	add_word(cmd, NULL, 0);
	rollback_tokens(cmd);
}
