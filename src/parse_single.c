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

char	*ft_concat(char *s1, char *s2)
{
	int i = 0;
	int j = 0;
	size_t len1 = 0;
	size_t len2 = 0;
	while (s1[len1] != '\0')
		len1++;
	while (s2[len2] != '\0')
		len2++;
	char *res = (char *)malloc(sizeof(char) * (len1+len2+2));
	while(s1[i] != '\0')
	{
		res[i] = s1[i];
		i++;
	}
	res[i++] = '\n';
	while (s2[j] != '\0')
	{
		res[i] = s2[j];
		i++;
		j++;
	}
	res[i] = 0;
	return (res);
}

char	*add_quote(char *s, char quote)
{
	char	*str;

	if (quote == '\'')
		str = readline("quote> ");
	else
		str = readline("dquote> ");
	s = ft_concat(s, str);
	free(str);
	return (s);
}

char	*check_quote(char *s)
{
	int	i;
	int	quoted;
	char	quote;
	
	i = 0;
	quoted = 0;
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '"') && quoted == 0)
		{
			quoted = 1;
			quote = s[i];
		}
		else if (quoted == 1 && s[i] == quote)
			quoted = 0;
		i++;
	}
	if (quoted == 1)
	{
		s = add_quote(s, quote);	
		s = check_quote(s);
	}
	return (s);	
}

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
	bck->str = malloc(ft_strlen(s) * sizeof(char));
	ft_printf("DEBUG : Parsing \x1b[32m %s\x1b[37m\n", s);
	while (s[i] == ' ')
		i++;
	while (s[i])
	{
		if (s[i] == '\\')
		{
			i++;
			bck->str[j++] = s[i];
		}
		else if (s[i] == '<' || s[i] == '>')
		{
			bck->str[j] = 0;
			bck->next = malloc(sizeof(t_word));
			bck = bck->next;
			bck->str = malloc(3 * sizeof(char));
			bck->str[j++] = s[i++];
			if (s[i] == s[i - 1])
				bck->str[j++] = s[i++];
			bck->str[j++] = 0;
			bck = bck->next;
			bck->str = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
		}
		else if (s[i] == ' ' && !quoted)
		{
			bck->str[j++] = s[i];
			bck->str[j++] = 0;
			bck->next = malloc(sizeof(t_word));
			bck = bck->next;
			bck->str = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
			bck->next = NULL;
			j = 0;
			while (s[i] == ' ')
				i++;
		}
		else if ((s[i] == '\'' || s[i] == '"') && !quoted)
		{
			quoted = 1;
			bck->str[j++] = 0;
			bck->next = malloc(sizeof(t_word));
			bck = bck->next;
			bck->quote = s[i];
			bck->str = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
			j = 0;
			i++;
		}
		else if ((s[i] == '\'' || s[i] == '"') && quoted && s[i] == bck->quote)
		{
			quoted = 0;
			bck->str[j++] = 0;
			bck->next = malloc(sizeof(t_word));
			bck = bck->next;
			bck->str = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
			j = 0;
			i++;
		}
		else 
			bck->str[j++] = s[i++];
	}
}


void	parse_with_pipes(char **s, t_word *c)
{
	int	i;
	t_word	*c_bck;

	c_bck = c;
	i = 0;
	while (s[i])
	{
		parse_single(s[i], c_bck);
		//c_bck = c_bck->pipe;
		i++;
	}
}
