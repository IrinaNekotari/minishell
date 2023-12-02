#include "minishell.h"

extern int	g_received_signal;

static int	emptiness(char *s, int i)
{
	while (is_whitespace(s[i]))
		i++;
	if (s[i])
		return (0);
	return (1);
}

static void	add_text_pipe(char **s)
{
	char	*str;

	super_concat(s, " ");
	str = readline("> ");
	while (str && emptiness(str, 0))
	{
		free(str);
		str = readline("> ");
	}
	if (!str)
	{
		free((*s));
		(*s) = NULL;
		return ;
	}
	super_concat(s, str);
	free(str);
}

//TODO : glhf pour le norminagement
char	*check_pipes(char *s)
{
	int		quote;
	char		quoted;
	int		i;
	int		u;

	i = 0;
	u = 0;
	quote = 0;
	quoted = 0;
	if (!s)
		return (NULL);
	while (is_whitespace(s[u]))
		u++;
	while (s[u + i])
	{
		if ((s[u + i] == '"' || s[u + i] == '\'') && quote == 0)
		{
			quote = 1;
			quoted = s[u + i];
		}
		else if (s[u + i] == quoted && quote == 1)
			quote = 0;
		else if (s[u + i] == '|' && quote == 0)
		{
			if ((i == 0 && s[u + i + 1] == '|') 
				|| (s[u + i + 1] == '|' && s[u + i + 2] == '|' && s[u + i + 3] == '|'))
			{
				error_syntax(ERROR, "||");
				free(s);
				return (NULL);
			}
			if (i == 0 || (s[u + i + 1] == '|' && s[u + i + 2] == '|'))
			{
				error_syntax(ERROR, "|");
				free(s);
				return (NULL);
			}
			if (s[u + i + 1] == '|' && s[u + i + 2])
			{
				g_received_signal = IGNORE_PIPES;
				i++;
			}
			if (s[u + i + 1] == '|' && emptiness(s, u + i + 2))
			{
				g_received_signal = IGNORE_PIPES;
				add_text_pipe(&s);
			}
			if (emptiness(s, u + i + 1))
				add_text_pipe(&s);
			if (!s)
				return (NULL);	
		}
		i++;
	}
	return (s);
}
