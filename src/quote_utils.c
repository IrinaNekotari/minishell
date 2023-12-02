/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 09:50:36 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/24 09:50:37 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_append(char *str, char c)
{
	int		i;
	char	*ret;

	i = 0;
	ret = ft_calloc(ft_strlen(str) + 2, sizeof(char));
	while (str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = c;
	return (ret);
}

char	*add_quote(char *s, char quote)
{
	char	*str;

	if (quote == '\'')
		str = readline("quote> ");
	else
		str = readline("dquote> ");
	if (!str)
		return (NULL);
	s = ft_concat(s, str);
	free(str);
	return (s);
}

static void	merde(char *s, int *i, int *quoted, char *quote)
{
	if ((s[*i] == '\'' || s[*i] == '"') && *quoted == 0)
	{
		*quoted = 1;
		*quote = s[*i];
	}
	else if (*quoted == 1 && s[*i] == *quote)
		*quoted = 0;
}

char	*check_quote(char *s)
{
	int		i;
	int		quoted;
	char	quote;

	i = 0;
	quoted = 0;
	quote = 0;
	while (s[i])
	{
		if (s[i] == '\\' && is_escapable(s[i + 1]))
			i++;
		else
			merde(s, &i, &quoted, &quote);
		i++;
	}
	if (quoted == 1)
	{
		s = add_quote(s, quote);
		if (!s)
			return (NULL);
		s = check_quote(s);
	}
	return (s);
}
