#include "minishell.h"

static int	search_for_double(t_cmd *cmd)
{
	int	ret;

	ret = 0;
	while (cmd->input->file)
	{
		if (cmd->input->io == DOUBLE_INPUT)
		{
			ret = 1;
			break ;
		}
		cmd->input = cmd->input->next;
	}
	while (cmd->input->previous)
		cmd->input = cmd->input->previous;
	return (ret);
}

//TODO C'EST QUOI CE CODE DE ZINZIN
static void	handle_double_input(t_cmd *cmd)
{
	char	*sortie;

	sortie = NULL;
	while (cmd->input->file)
		cmd->input = cmd->input->next;
	while (cmd->input->previous)
	{
		if (cmd->input->io == DOUBLE_INPUT)
		{
			sortie = readline("> ");
			while(!ft_equals(sortie, cmd->input->file))
			{
				free(sortie);
				sortie = readline("> ");
			}
			free(sortie);
		}
		cmd->input = cmd->input->previous;
	}
	cmd->input = cmd->input->next;
	while (cmd->input->file)
	{
		if (cmd->input->io == DOUBLE_INPUT)
		{
			sortie = readline("> ");
			while(!ft_equals(sortie, cmd->input->file))
			{
				free(sortie);
				sortie = readline("> ");
			}
			free(sortie);
		}
		cmd->input = cmd->input->next;
	}
	while (cmd->input->previous)
		cmd->input = cmd->input->previous;
}

static int	handle_input(t_cmd *cmd)
{
	int	fd;

	if (search_for_double(cmd))
		handle_double_input(cmd);
	while(cmd->input->file)
	{
		fd = open(cmd->input->file, O_RDONLY);
		if (cmd->input->io == SINGLE_INPUT && fd == -1)
		{
			ft_printf("[ERROR] : No such file or directory \"%s\"\n", cmd->input->file);
			return (0);
		}
		close(fd);
		cmd->input = cmd->input->next;
	}
	return (1);
}

static void	handle_output(t_cmd *cmd, char *str)
{
	int	fd;

	while (cmd->output->file)
	{
		if (cmd->output->next->file)
			fd = open(cmd->output->file, O_CREAT 
					| O_RDWR | O_TRUNC);
		else
		{
			if (cmd->output->io == SINGLE_INPUT)
			{
				fd = open(cmd->output->file, O_CREAT 
					| O_RDWR | O_TRUNC);
				ft_printf("BOOM !\n");
			}
			else
			{
				fd = open(cmd->output->file, O_CREAT 
					| O_RDWR | O_APPEND);
				ft_printf("BIM !\n");
			}
			ft_putstr_fd(str, fd);
		}
		close(fd);
		cmd->output = cmd->output->next;
	}
}

/**
* Prends un message sous forme de char *,
* et l'imprime dans un fichier si un output
* est indique, et a la console sinon et si
* la commande n'as pas de pipe suivante
*/
void	print_io(t_cmd *cmd, char *str)
{
	if (cmd->input->file)
	{
		if (!handle_input(cmd))
			return ;
	}
	if (cmd->output->file)
		handle_output(cmd, str);
	else if (!cmd->pipe)
		ft_printf("%s\n", str);
}
