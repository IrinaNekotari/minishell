#include "minishell.h"

void	handle_output(t_cmd *cmd, char *str)
{
	int	fd;

	while (cmd->output->file)
	{
		if (cmd->output->next->file)
			fd = open(cmd->output->file, O_CREAT 
					| O_RDWR | O_TRUNC, 0777);
		else
		{
			if (cmd->output->io == SINGLE_OUTPUT)
				fd = open(cmd->output->file, O_CREAT 
					| O_RDWR | O_TRUNC, 0777);
			else
				fd = open(cmd->output->file, O_CREAT 
					| O_RDWR | O_APPEND, 0777);
			ft_putstr_fd(str, fd);
		}
		close(fd);
		cmd->output = cmd->output->next;
	}
}
