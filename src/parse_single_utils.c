/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_single_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 15:15:13 by nmascrie          #+#    #+#             */
/*   Updated: 2023/12/05 15:15:14 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_tokens(t_cmd **cmd, char *str, char quote, char next)
{
	(*cmd)->tokens = ft_calloc(1, sizeof(t_word));
	(*cmd)->tokens->next = NULL;
	(*cmd)->tokens->previous = NULL;
	if (str)
		(*cmd)->tokens->str = ft_strdup(str);
	else
		(*cmd)->tokens->str = NULL;
	(*cmd)->tokens->quote = quote;
	if (is_whitespace(next))
		(*cmd)->tokens->has_space = 1;
	else
		(*cmd)->tokens->has_space = 0;
}

void	add_word(t_cmd **cmd, char *str, char quote, char next)
{
	if (!(*cmd)->tokens)
		init_tokens(cmd, str, quote, next);
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
		if (is_whitespace(next))
			(*cmd)->tokens->has_space = 1;
		else
			(*cmd)->tokens->has_space = 0;
	}
}

char	*get_next_word(char *str, int *i, char delim)
{
	int		j;
	char	*ret;

	j = 0;
	ret = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	while (str[*i])
	{
		if (str[*i] == '\\' && (i == 0 || str[*i - 1] != '\\')
			&& delim != '\'' && str[*i + 1] != '$')
			(*i) += 1;
		if (!str[*i])
			break ;
		if (delim == 0 && delimm(str, i))
			break ;
		else if (str[*i] == delim && (i == 0 || str[*i - 1] != '\\'))
			break ;
		ret[j] = str[*i];
		j++;
		(*i) += 1;
	}
	return (ret);
}

void	handle_chevrons_2(char *t, char *s, int *i)
{
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
}

void	handle_chevrons(t_cmd **cmd, char *s, int *i)
{
	char	*t;

	t = ft_calloc(4, sizeof(char));
	if ((*i) > 0 && s[(*i) - 1] == '\\')
	{
		t[0] = '\\';
		t[1] = s[*i];
		(*i) += 1;
		add_word(cmd, t, 0, s[*i]);
		free(t);
		return ;
	}
	t[0] = s[*i];
	(*i) += 1;
	handle_chevrons_2(t, s, i);
	add_word(cmd, t, 0, s[*i]);
	free(t);
}
