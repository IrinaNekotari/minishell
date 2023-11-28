/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:18:32 by mjuette           #+#    #+#             */
/*   Updated: 2023/10/31 14:18:33 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_error(char *str)
{
	int	i;
	int	j;

	i = 0;
	while (is_whitespace(str[i]))
		i++;
	if (str[i] == '|')
	{
		ft_printf("parse error near `|'\n");
		return (0);
	}
	while (str[i])
	{
		/* pipe error*/
		if (str[i] == '|' && str[i + 1] == '|' && str[i + 2] == '|'
			&& i < (int)ft_strlen(str) - 2)
		{
			ft_printf("parse error near `||'\n");
			return (0);
		}
		else if (str[i] == '|')
		{
			j = i + 1;
			while (is_whitespace(str[j]))
				j++;
			if (j == (int)ft_strlen(str))
			{
				ft_printf("c'est pas traite lol\n");
				return (0);
			}
			if (str[j] == '|')
			{
				ft_printf("c'est pas traitedddddddd lol\n");
				return (0);
			}
		}
		//TODO chevron error
		if (str[i] == '>' && str[i + 1] == '>'
			&& str[i + 2] == '>' && i < (int)ft_strlen(str) - 2)
		{
			ft_printf("parse error near `||'\n");
			return (0);
		}
		else if (str[i] == '>')
		{
			j = i + 1;
			while (is_whitespace(str[j]))
				j++;
			if (j == (int)ft_strlen(str))
			{
				ft_printf("c'est pas traite lol\n");
				return (0);
			}
			if (str[j] == '>')
			{
				ft_printf("c'est pas traitedddddddd lol\n");
				return (0);
			}
		}
		if (str[i] == '<' && str[i + 1] == '<'
			&& str[i + 2] == '<' && i < (int)ft_strlen(str) - 2)
		{
			ft_printf("parse error near `||'\n");
			return (0);
		}
		else if (str[i] == '<')
		{
			j = i+1;
			while (is_whitespace(str[j]))
				j++;
			if (j == (int)ft_strlen(str))
			{
				ft_printf("c'est pas traite lol\n");
				return (0);
			}
			if (str[j] == '<')
			{
				ft_printf("c'est pas drole\n");
				return (0);
			}
		}
		//permission denied
		if (str[i] == '"' && str[i + 1] == '"')
		{
			ft_printf("technique interdite\n");
			return (0);
		}
		if (str[i] == '\'' && str[i + 1] == '\'')
		{
			ft_printf("technique interdite\n");
			return (0);
		}
		i++;
	}
	if (str[ft_strlen(str)-1] == '|')
	{
		ft_printf("c'est pas traite lol\n");
		return (0);
	}
	return (1);
}
