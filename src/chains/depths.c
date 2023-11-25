#include "minishell.h"

int	input_depth(t_cmd *cmd)
{
	int	depth;

	depth = 0;
	while (cmd->input->file)
	{
		depth++;
		cmd->input = cmd->input->next;
	}
	while (cmd->input->previous)
		cmd->input = cmd->input->previous;
	return (depth);
}

int	output_depth(t_cmd *cmd)
{
	int	depth;

	depth = 0;
	while (cmd->output->file)
	{
		depth++;
		cmd->output = cmd->output->next;
	}
	while (cmd->output->previous)
		cmd->output = cmd->output->previous;
	return (depth);
}

int	tokens_depth(t_cmd *cmd)
{
	int	depth;

	depth = 0;
	while (cmd->tokens->str)
	{
		depth++;
		cmd->tokens = cmd->tokens->next;
	}
	while (cmd->tokens->previous)
		cmd->tokens = cmd->tokens->previous;
	return (depth);	
}
