#include "minishell.h"

/**
* Prends un message sous forme de char *,
* et l'imprime dans un fichier si un output
* est indique, et a la console sinon et si
* la commande n'as pas de pipe suivante
*/
void	print_io(t_cmd *cmd, char *str)
{
	handle_output_create(cmd);
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
