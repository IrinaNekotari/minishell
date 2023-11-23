/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 09:53:31 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/24 09:53:32 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_usable(char c)
{
	if (ft_isalpha(c))
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

int	is_delim(char c)
{
	if (c == '"')
		return (1);
	if (c == '\'')
		return (1);
	if (c == '<')
		return (1);
	if (c == '>')
		return (1);
	if (c == '|')
		return (1);
	return (0);
}

int	is_whitespace(char c)
{
	if (c == ' ')
		return (1);
	if (c == '\t')
		return (1);
	if (c == '\r')
		return (1);
	if (c == '\n')
		return (1);
	if (c == '\v')
		return (1);
	if (c == '\f')
		return (1);
	return (0);
}

//TODO : Check pour tous les \\ \" \' etc
int	is_escapable(char c)
{
	if (c == '\\')
		return (1);
	if (c == '\'')
		return (1);
	if (c == '"')
		return (1);
	if (c == '|')
		return (1);
	if (c == ';')
		return (1);
	if (c == '<')
		return (1);
	if (c == '>')
		return (1);
	return (0);
}

int	is_escapable2(char c)
{
	if (c == '\\')
		return (1);
	if (c == '\'')
		return (1);
	if (c == '"')
		return (1);
	if (c == ';')
		return (1);
	return (0);
}
