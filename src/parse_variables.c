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
		a++;
	}
	*i += a;
	return (ret);
}

void	found_the_flame(char **ret, int *j, char *val)
{
	int	i;

	i = 0;
	if (val)
	{
		while (val[i])
		{
			(*ret)[*j] = val[i];
			*j += 1;
			i++;
		}
		free(val);
	}
}

//TODO : Chercher la bonne taille du calloc (ultra cancer)
//TODO : Norminer (encore plus cancer)
//TODO : On a un invalid read a la ligne 51 (Ultra Super Cancer Sayan 3)
char	*get_variables(char *str, t_main *main)
{
	char	*ret;
	char	*get;
	char	*val;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ret = ft_calloc(ft_strlen(str) * 10, sizeof(char));
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			val = ft_itoa(main->last);
			i += 2;
			found_the_flame(&ret, &j, val);
		}
		else if (str[i] == '$' && is_usable(str[i + 1]))
		{
			get = find_the_flame(&i, str);
			val = ft_getenv(main->env, get);
			found_the_flame(&ret, &j, val);
			if (get)
				free(get);
		}
		else
			ret[j] = str[i];
		if (!str[i])
			break ;
		j++;
		i++;
	}
	return (ret);
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
			search = get_variables((*cmd)->tokens->str, (*main));
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
