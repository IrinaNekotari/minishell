/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 09:01:08 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/24 09:01:09 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	log_parse_single(char *str)
{
	int	fd;

	if (LOGGER)
	{
		ft_printf("[\x1b[33mDEBUG\x1b[37m] : ");
		ft_printf("💩️ Parsing \x1b[32m %s\x1b[37m\n", str);
	}
	if (LOG_TO_FILE)
	{
		fd = open("log", O_RDWR | O_APPEND | O_CREAT);
		ft_putstr_fd("[DEBUG]	Parsing line : ", fd);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("\n", fd);
		close(fd);
	}
}

void	log_input(char	*str)
{
	int	fd;

	if (LOGGER)
	{
		ft_printf("[\x1b[33mDEBUG\x1b[37m][\x1b[33mINPUT\x1b[37m] : ");
		ft_printf("\x1b[32m %s\x1b[37m\n", str);
	}
	if (LOG_TO_FILE)
	{
		fd = open("log", O_RDWR | O_APPEND | O_CREAT);
		ft_putstr_fd("[USER]		User input : ", fd);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("\n", fd);
		close(fd);
	}
}

void	log_open_exit(int i)
{
	int	fd;

	if (LOG_TO_FILE)
	{
		fd = open("log", O_RDWR | O_APPEND | O_CREAT);
		if (i == 1)
			ft_putstr_fd("[MINISHELL]	Successfully launched\n", fd);
		else
			ft_putstr_fd("[MINISHELL]	Successfully closed\n", fd);
		close(fd);
	}
}
