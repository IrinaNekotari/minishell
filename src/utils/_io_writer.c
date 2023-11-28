/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _io_writer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 13:45:16 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/28 13:45:17 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

/**
* Prends un message sous forme de char *,
* et l'imprime dans un fichier si un output
* est indique, et a la console sinon et si
* la commande n'as pas de pipe suivante
*/
void	print_io(t_cmd *cmd, char *str, t_main **main)
{
	handle_output_create(cmd);
	if (cmd->input->file)
	{
		if (!handle_input(cmd))
			return ;
	}
	if (cmd->output->file)
		handle_output(cmd, str);
	else if (!cmd->pipe && g_received_signal == IGNORE_NEW_LINE)
		ft_printf("%s", str);
	else if (!cmd->pipe)
		ft_printf("%s\n", str);
	ft_putstr_fd(str, (*main)->pipes[0]);
	if ((*main)->inpipe)
		free((*main)->inpipe);
	(*main)->inpipe = ft_strdup(str);
}
