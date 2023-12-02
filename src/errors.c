/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 13:43:40 by nmascrie          #+#    #+#             */
/*   Updated: 2023/11/21 13:43:42 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_exec(int err)
{
	char	*r;

	r = strerror(err);
	ft_printf("[\x1b[31mERROR\x1b[37m] \x1b[33m%s\x1b[37m\n", r);
}

int	error_syntax(int severity, char *loc)
{
	if (severity == INFO)
		ft_printf("[\x1b[32mNOTICE\x1b[37m]");
	else if (severity == WARNING)
		ft_printf("[\x1b[33mWARNING\x1b[37m]");
	else if (severity == ERROR)
		ft_printf("[\x1b[31mERROR\x1b[37m]");
	else if (severity == CRITICAL)
		ft_printf("[\x1b[35mCRITICAL !!!\x1b[37m]");
	else if (severity == FUBAR)
		ft_printf("[\x1b[1m\x1b[31mHOLY SHIT\x1b[0m\x1b[37m]");
	else
		ft_printf("[DEBUG\x1b[37m]");
	ft_printf(" Syntax error near unexpected token \'%s\'\n", loc);
	return (0);
}

void	error_print(int severity, char *msg, char *add)
{
	if (severity == INFO)
		ft_printf("[\x1b[32mNOTICE\x1b[37m]");
	else if (severity == WARNING)
		ft_printf("[\x1b[33mWARNING\x1b[37m]");
	else if (severity == ERROR)
		ft_printf("[\x1b[31mERROR\x1b[37m]");
	else if (severity == CRITICAL)
		ft_printf("[\x1b[35mCRITICAL !!!\x1b[37m]");
	else if (severity == FUBAR)
		ft_printf("[\x1b[1m\x1b[31mHOLY SHIT\x1b[0m\x1b[37m]");
	else
		ft_printf("[DEBUG\x1b[37m]");
	if (add)
		ft_printf(" %s : %s\n", add, msg);
	else
		ft_printf(" %s\n", msg);
}
