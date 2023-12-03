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
	else
		ft_printf("%s", str);
	(void)main;
}

static int	get_last_output(t_cmd *cmd)
{
	int	fd;

	while (cmd->output->file)
	{
		if (!cmd->output->next || !cmd->output->next->file)
			break;
		cmd->output = cmd->output->next;
	}
	if (cmd->output->io == SINGLE_OUTPUT)
		fd = open(cmd->output->file, O_CREAT
				| O_RDWR | O_TRUNC, 0777);
	else
		fd = open(cmd->output->file, O_CREAT
				| O_RDWR | O_APPEND, 0777);
	return (fd);
}

void	io_pipe(t_cmd *cmd, t_main **main)
{
	int	fd;

	handle_output_create(cmd);
	if (cmd->input->file)
	{
		if (!handle_input(cmd))
			exit (SIGNAL_ABORT);
	}
	fd = get_last_output(cmd);
	ft_putstr_fd("", 1);
	if (dup2(fd, 1) == -1 || (cmd->pipe && (close((*main)->pipes[0]) == -1
			|| close((*main)->pipes[1]) == -1
			|| close(fd == -1))))
	{
		error_print(CRITICAL, "An error has occured while piping !", NULL);
		exit(-1);
	}
}
