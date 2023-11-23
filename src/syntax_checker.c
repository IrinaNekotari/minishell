#include "minishell.h"

static int	is_chevrons(char *str, char quote)
{
	if (!str || quote != 0)
		return (0);
	if (ft_equals(str, ">"))
		return (1);
	if (ft_equals(str, "<"))
		return (1);
	if (ft_equals(str, ">>"))
		return (1);
	if (ft_equals(str, "<<"))
		return (1);
	if (ft_equals(str, "<>"))
		return (1);
	return (0);
}

void	insert_token(t_cmd **cmd, char *str, int q)
{
	t_word	*t;

	t = ft_calloc(1, sizeof(t_word));
	t->previous = (*cmd)->tokens;
	t->next = (*cmd)->tokens->next;
	t->str = ft_strdup(str);
	t->quote = q;
	(*cmd)->tokens->next->previous = t;
	(*cmd)->tokens->next = t;
	(*cmd)->tokens = (*cmd)->tokens->next;
}

static int	exception_de_merde(t_cmd **cmd)
{
	if (!(*cmd)->tokens->next)
		return (error_syntax(ERROR, "newline"));
	else if (is_chevrons((*cmd)->tokens->next->str, (*cmd)->tokens->next->quote))
		return (error_syntax(ERROR, (*cmd)->tokens->next->str));
	free((*cmd)->tokens->str);
	(*cmd)->tokens->str = ft_strdup("<");
	insert_token(cmd, (*cmd)->tokens->next->str, 
			(*cmd)->tokens->next->quote);
	insert_token(cmd, ">", 0);
	return (1);
}		

int	check_chevrons(t_cmd **cmd)
{
	while ((*cmd)->tokens->str)
	{
		//ft_printf("Got %s\n", (*cmd)->tokens->str);
		if (ft_equals((*cmd)->tokens->str, "<>") && !(*cmd)->tokens->quote)
		{
			if (!exception_de_merde(cmd))
				return (0);
		}
		if (is_chevrons((*cmd)->tokens->str, (*cmd)->tokens->quote))
		{
			if (!(*cmd)->tokens->next->str)
				return (error_syntax(ERROR, "newline"));
			else if (is_chevrons((*cmd)->tokens->next->str, (*cmd)->tokens->next->quote))
				return (error_syntax(ERROR, (*cmd)->tokens->next->str));
		}
		(*cmd)->tokens = (*cmd)->tokens->next;
	}
	while ((*cmd)->tokens->previous)
		(*cmd)->tokens = (*cmd)->tokens->previous;
	return (1);
}
