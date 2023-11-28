#include "minishell.h"

int	is_io(char *s)
{
	if (ft_equals(s, ">"))
		return (SINGLE_OUTPUT);
	else if (ft_equals(s, ">>"))
		return (DOUBLE_OUTPUT);
	else if (ft_equals(s, "<"))
		return (SINGLE_INPUT);
	else if (ft_equals(s, "<<"))
		return (DOUBLE_INPUT);
	else
		return (0);
}


void	slash_tokens(t_cmd **cmd)
{
	t_word	*bck;

	bck = (*cmd)->tokens;
	(*cmd)->tokens->previous->next = (*cmd)->tokens->next;
	if ((*cmd)->tokens->str)
		free((*cmd)->tokens->str);
	(*cmd)->tokens->next->previous = (*cmd)->tokens->previous;
	(*cmd)->tokens = (*cmd)->tokens->next;
	free(bck);
}

//TODO : NETTOYER CETTE MEEEEEEEEEEERDE
void	generate_io(t_cmd **cmd)
{
	int	i;

	i = 0;
	rollback_io(cmd);
	rollback_tokens(cmd);
	(*cmd)->input = ft_calloc(1, sizeof(t_io));
	(*cmd)->input->previous = NULL;
	(*cmd)->output = ft_calloc(1, sizeof(t_io));
	(*cmd)->output->previous = NULL;
	while ((*cmd)->tokens->str)
	{
		if (is_io((*cmd)->tokens->str) && !(*cmd)->tokens->quote)
		{
			i = is_io((*cmd)->tokens->str);
			slash_tokens(cmd);
			if (i == SINGLE_OUTPUT || i == DOUBLE_OUTPUT)
			{
				(*cmd)->output->file = ft_strdup((*cmd)->tokens->str);
				(*cmd)->output->io = i;
				(*cmd)->output->next = ft_calloc(1, sizeof(t_io));
				(*cmd)->output->next->previous = (*cmd)->output;
				(*cmd)->output = (*cmd)->output->next;
				(*cmd)->output->file = NULL;
			}
			else
			{
				(*cmd)->input->file = ft_strdup((*cmd)->tokens->str);
				(*cmd)->input->io = i;
				(*cmd)->input->next = ft_calloc(1, sizeof(t_io));
				(*cmd)->input->next->previous = (*cmd)->input;
				(*cmd)->input = (*cmd)->input->next;
				(*cmd)->input->file = NULL;
			}
			slash_tokens(cmd);
		}
		else
			(*cmd)->tokens = (*cmd)->tokens->next;
	}
	rollback_io(cmd);
	rollback_tokens(cmd);
	if ((*cmd)->pipe)
		generate_io(&(*cmd)->pipe);
}
