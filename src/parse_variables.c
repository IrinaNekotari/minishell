/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_variables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:51:13 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/28 14:51:14 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_the_flame(int *i, char *str)
{
	int		a;
	char	*ret;

	a = 0;
	*i += 1;
	ret = ft_calloc(ft_strlen(str), sizeof(char));
	while (str[*i + a] && !is_whitespace(str[*i + a]))
	{
		ret[a] = str[*i + a];
		if (str[*i + a + 1] == '"' || str[*i + a + 1] == '\''
			|| !str[*i + a + 1] || is_whitespace(str[*i + a + 1]))
			break ;
		a++;
	}
	*i += a;
	return (ret);
}

void	found_the_flame(char **ret, int *j, char *val)
{
	int	k;

	k = 0;
	if (val)
	{
		while (val[k])
		{
			(*ret)[*j] = val[k];
			if (!val[k + 1])
				break ;
			*j += 1;
			k++;
		}
		free(val);
	}
}

static void	check_dollar(t_main *main, t_strings *strings, char *str)
{
	if (str[strings->i] == '$' && str[strings->i + 1] == '?'
		&& (strings->i == 0 || str[strings->i - 1] != '\\'))
	{
		strings->val = ft_itoa(main->last);
		strings->i += 2;
		found_the_flame(&strings->ret, &strings->j, strings->val);
	}
	else if (str[strings->i] == '$' && is_usable(str[strings->i + 1])
		&& (strings->i == 0 || str[strings->i - 1] != '\\'))
	{
		strings->get = find_the_flame(&strings->i, str);
		strings->val = ft_getenv(main->env, strings->get);
		found_the_flame(&strings->ret, &strings->j, strings->val);
		if (strings->get)
			free(strings->get);
	}
	else
		strings->ret[strings->j] = str[strings->i];
}

char	*get_variables(char *str, t_main *main)
{
	t_strings	strings;

	strings.i = 0;
	strings.j = 0;
	strings.ret = ft_calloc(ft_strlen(str)
			+ env_length(main->env), sizeof(char));
	while (str[strings.i])
	{
		check_dollar(main, &strings, str);
		if (!str[strings.i])
			break ;
		strings.j++;
		strings.i++;
	}
	return (strings.ret);
}

void	generate_variables(t_cmd **cmd, t_main **main)
{
	char	*search;

	while ((*cmd)->tokens->str)
	{
		if (ft_equals((*cmd)->tokens->next->str, " "))
		{
			rollback_tokens(cmd);
			return ;
		}
		if ((*cmd)->tokens->quote != '\'')
		{
			search = get_variables((*cmd)->tokens->str, *main);
			free((*cmd)->tokens->str);
			(*cmd)->tokens->str = ft_strdup(search);
			free(search);
		}
		if (!(*cmd)->tokens->next || !(*cmd)->tokens->next->str)
			break ;
		(*cmd)->tokens = (*cmd)->tokens->next;
	}
	if ((*cmd)->pipe)
		generate_variables(&((*cmd)->pipe), main);
}
