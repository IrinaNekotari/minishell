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

void	parse_single(char *s, t_word *c)
{
	int	i;
	int	j;
	int	quoted;
	t_word	*bck;

	bck = c;
	i = 0;
	j = 0;
	quoted = 0;
	bck->str = (char *)ft_calloc((ft_strlen(s) + 1), sizeof(char));
	//TODO : Nettoyer cette merde*/
	(void)c;
	log_parse_single(s);
	while (s[i] == ' ')
		i++;
	while (s[i])
	{
		if (s[i] == '\\')
		{
			if (is_escapable(s[i + 1]))
			{
				i++;
				bck->str[j++] = s[i++];
			}
			else
				bck->str[j++] = s[i++];
		}
		else if (s[i] == '<' || s[i] == '>')
		{
			if (is_blank(s, i))
				break ;
			if (i >= 1 && !is_whitespace(s[i - 1]))
			{
				bck->next = ft_calloc(1, sizeof(t_word));
				bck = bck->next;
				bck->str = ft_calloc(3, sizeof(char));
				j = 0;
			}
			bck->str[j++] = s[i++];
			if (s[i] == s[i - 1])
				bck->str[j++] = s[i++];
			while (is_whitespace(s[i]))
				i++;
			bck->next = ft_calloc(1, sizeof(t_word));
			bck = bck->next;
			bck->str = ft_calloc(3, sizeof(char));
			j = 0;
		}
		else if (is_whitespace(s[i]) && !quoted)
		{
			if (is_blank(s, i))
				break ;
			if (i >= 1 && !is_whitespace(s[i - 1]))
			{
				bck->next = ft_calloc(1, sizeof(t_word));
				bck = bck->next;
				bck->str = ft_calloc(3, sizeof(char));
				j = 0;
			}
			while (is_whitespace(s[i]))
				i++;
			if (s[i] == '"' || s[i] == '\'')
			{
				quoted = 1;
				bck->quote = s[i];
				i++;
			}
			bck->next = ft_calloc(1, sizeof(t_word));
			bck = bck->next;
			bck->str = ft_calloc(3, sizeof(char));
			j = 0;
		}
		else if ((s[i] == '\'' || s[i] == '"') && !quoted)
		{
			quoted = 1;
			if (is_blank(s, i))
				break ;
			if (i >= 1 && !is_whitespace(s[i - 1]))
			{
				bck->next = ft_calloc(1, sizeof(t_word));
				bck = bck->next;
				bck->str = ft_calloc(3, sizeof(char));
				j = 0;
			}
			bck->quote = s[i];
			i++;
		}
		else if ((s[i] == '\'' || s[i] == '"') && quoted && s[i] == bck->quote)
		{
			quoted = 0;
			if (is_blank(s, i))
				break ;
			if (i >= 1 && !is_whitespace(s[i - 1]))
			{
				bck->next = ft_calloc(1, sizeof(t_word));
				bck = bck->next;
				bck->str = ft_calloc(3, sizeof(char));
				j = 0;
			}
			i++;
			while (is_whitespace(s[i]))
				i++;
			bck->next = ft_calloc(1, sizeof(t_word));
			bck = bck->next;
			bck->str = ft_calloc(3, sizeof(char));
			j = 0;
		}
		else 
			bck->str[j++] = s[i++];
	}
	bck->str[j++] = 0;
	bck->next = NULL;
}

void	ft_trimword(t_word *c)
{
	int	i;
	t_word *word;
	
	i = 0;
	word = c;
	if(word->str)
	{
		while(is_whitespace(word->str[i]))
			i++;
		if(!(word->str[i] == '\0'))
			return ;
		free(word->str);
	}
	word->str = NULL;
	word->next = NULL;
	free(word);
}

int	is_blank(char *ptr, int j)
{
	int	i;
	i = 0;
	while (i < j)
		i++;
	while (ptr[i])
	{
		if (!is_whitespace(ptr[i]))
			return (0);
		i++;
	}
	return (1);
}
