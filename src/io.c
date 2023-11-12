#include "minishell.h"

# define SINGLE_OUTPUT 1
# define DOUBLE_OUTPUT 2
# define SINGLE_INPUT 3
# define DOUBLE_INPUT 4

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

void	slash_tokens(t_word *w)
{
	t_word	*bck;

	bck = w;
	if (w->str)
		free(w->str);
	w = w->next;
	free(bck);
}


void	generate_io(t_cmd *cmd)
{
	t_cmd	*bck;
	int	i;

	i = 0;
	bck = cmd;
	bck->input = ft_calloc(1 , sizeof(t_io));
	bck->output = ft_calloc(1 , sizeof(t_io));
	while (bck->tokens)
	{
		if (is_io(bck->tokens->str) && !bck->tokens->quote)
		{
			i = is_io(bck->tokens->str);
			slash_tokens(bck->tokens);
			bck->tokens = bck->tokens->next;
			if (i == SINGLE_OUTPUT || i == DOUBLE_OUTPUT)
			{
				bck->output->file = ft_strdup(bck->tokens->str);
				bck->output->io = i;
				bck->output->next = ft_calloc(1 , sizeof(t_io));
				bck->output = bck->output->next;
			}
			else
			{
				bck->input->file = ft_strdup(bck->tokens->str);
				bck->input->io = i;
				bck->input->next = ft_calloc(1 , sizeof(t_io));
				bck->input = bck->output->next;
			}
			slash_tokens(bck->tokens);
		}
		else
			bck->tokens = bck->tokens->next;
	}
	if (bck->pipe)
		generate_io(bck->pipe);
}
