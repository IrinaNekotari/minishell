/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:20:29 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/28 14:20:31 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_cmd *cmd, t_main *main)
{
	char	*dir;

	dir = ft_getenv(main->env, "PWD");
	if (!dir)
		dir = ft_strdup("No PWD found !\n");
	else
		super_concat(&dir, "\n");
	print_io(cmd, dir, &main);
	free(dir);
}
