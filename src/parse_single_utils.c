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
