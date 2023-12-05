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

static void	error_level(int severity)
{
	if (severity == INFO)
		ft_putstr_fd("[\x1b[32mNOTICE\x1b[37m] ", 2);
	else if (severity == WARNING)
		ft_putstr_fd("[\x1b[33mWARNING\x1b[37m] ", 2);
	else if (severity == ERROR)
		ft_putstr_fd("[\x1b[31mERROR\x1b[37m] ", 2);
	else if (severity == CRITICAL)
		ft_putstr_fd("[\x1b[35mCRITICAL\x1b[37m] ", 2);
	else if (severity == FUBAR)
		ft_putstr_fd("[\x1b[1m\x1b[31mHOLY SHIT\x1b[0m\x1b[37m] ", 2);
	else
		ft_putstr_fd("[DEBUG\x1b[37m] ", 2);
}

void	error_env(void)
{
	error_level(INFO);
	ft_putstr_fd("You're trying to delete or update a core ", 2);
	ft_putstr_fd("Environnement variable.\n", 2);
	error_level(INFO);
	ft_putstr_fd("Doing so might lead to unexpected ", 2);
	ft_putstr_fd("results or crashes.\n", 2);
}

void	error_exec(int err)
{
	char	*r;

	r = strerror(err);
	error_level(ERROR);
	ft_putstr_fd(r, 2);
	ft_putstr_fd("\n", 2);
}

int	error_syntax(int severity, char *loc)
{
	error_level(severity);
	ft_putstr_fd(" Syntax error near unexpected token \'", 2);
	ft_putstr_fd(loc, 2);
	ft_putstr_fd("\'\n", 2);
	return (0);
}

void	error_print(int severity, char *msg, char *add)
{
	error_level(severity);
	ft_putstr_fd(msg, 2);
	if (add)
	{
		ft_putstr_fd(" : ", 2);
		ft_putstr_fd(add, 2);
	}
	ft_putstr_fd("\n", 2);
}
