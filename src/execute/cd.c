/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:54:52 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/23 13:54:55 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cd(t_cmd *cmd, t_main **main)
{
	int		dd;
	char	*buffer;

	dd = chdir(cmd->tokens->next->str);
	if (dd == -1)
	{
		error_exec(errno);
		return ;
	}
	buffer = ft_calloc(250, sizeof(char));
	getcwd(buffer, 250);
	update_env(&((*main)->env), "PWD", buffer);
	free(buffer);
}

void	ft_cd(t_cmd *cmd, t_main **main)
{
	char	*buffer;

	if (cmd->tokens->next && cmd->tokens->next->next
		&& cmd->tokens->next->str && cmd->tokens->next->next->str)
	{
		error_print(ERROR, "Too many arguments", NULL);
		return ;
	}
	else if (!cmd->tokens->next || !cmd->tokens->next->str)
	{
		buffer = ft_strdup((*main)->initpwd);
		update_env(&((*main)->env), "PWD", buffer);
		chdir(buffer);
		free(buffer);
	}
	else
		exec_cd(cmd, main);
}
