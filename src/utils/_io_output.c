#include "minishell.h"

extern int	g_received_signal;

void	handle_output_create(t_cmd *cmd)
{
	int	fd;

	while (cmd->output->file)
	{
		if (cmd->output->io == SINGLE_OUTPUT)
			fd = open(cmd->output->file, O_CREAT 
				| O_RDWR | O_TRUNC, 0777);
		else
			fd = open(cmd->output->file, O_CREAT 
				| O_RDWR | O_APPEND, 0777);
		close(fd);
		if (!cmd->output->next->file)
			break ;
		cmd->output = cmd->output->next;
	}
}

void	handle_output(t_cmd *cmd, char *str)
{
	int	fd;

	if (cmd->output->io == SINGLE_OUTPUT)
		fd = open(cmd->output->file, O_CREAT 
			| O_RDWR | O_TRUNC, 0777);
	else
		fd = open(cmd->output->file, O_CREAT 
			| O_RDWR | O_APPEND, 0777);
	ft_putstr_fd(str, fd);
	if (g_received_signal != IGNORE_NEW_LINE)
		ft_putstr_fd("\n", fd);
	close(fd);
	cmd->output = cmd->output->next;
}
