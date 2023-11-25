#include "minishell.h"

/**
* Retourne la taille du nom de fichier le plus long dans l'input
*/
int	input_length(t_cmd *cmd)
{
	size_t	len;

	len = 0;
	while (cmd->input->file)
	{
		if (ft_strlen(cmd->input->file) > len)
			len = ft_strlen(cmd->input->file);
		cmd->input = cmd->input->next;
	}
	while (cmd->input->previous)
		cmd->input = cmd->input->previous;
	return ((int)len);
}

/**
* Retourne la taille du nom de fichier le plus long dans l'output
*/
int	output_length(t_cmd *cmd)
{
	size_t	len;

	len = 0;
	while (cmd->output->file)
	{
		if (ft_strlen(cmd->output->file) > len)
			len = ft_strlen(cmd->output->file);
		cmd->output = cmd->output->next;
	}
	while (cmd->output->previous)
		cmd->output = cmd->output->previous;
	return ((int)len);
}

/**
* Retourne la taille du string le plus long dans les tokens
*/
int	tokens_length(t_cmd *cmd)
{
	size_t	len;

	len = 0;
	while (cmd->tokens->str)
	{
		if (ft_strlen(cmd->tokens->str) > len)
			len = ft_strlen(cmd->tokens->str);
		cmd->tokens = cmd->tokens->next;
	}
	while (cmd->output->previous)
		cmd->output = cmd->output->previous;
	return ((int)len);
}