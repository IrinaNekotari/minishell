#include "minishell.h"

static void	super_concat(char **a, char *b)
{
	char	*bck;

	bck = ft_concat2((*a), b);
	free((*a));
	(*a) = ft_strdup(bck);
	free(bck);
}

static char	*create_list(t_env *env)
{
	char *a;

	a = ft_calloc(1, sizeof(char));
	while (env)
	{
		super_concat(&a, env->name);
		super_concat(&a, "=");
		super_concat(&a, env->value);
		if (env->next)
			super_concat(&a, "\n");
		env = env->next;
	}
	return (a);
}

void	ft_env(t_cmd *cmd, t_env *env)
{
	char	*vars;

	if (!cmd->tokens->next->str)
	{
		vars = create_list(env);
		print_io(cmd, vars);
		free(vars);
	}
	else
	{
		vars = create_list(env);
		cmd->tokens = cmd->tokens->next;
		while (cmd->tokens->str)
		{
			if(!ft_strchr(cmd->tokens->str, '='))
			{
				handle_output_create(cmd);
				ft_printf("[\x1b[31mENV ERROR\x1b[37m] : \"%s\" is not a file or a directory !\n", cmd->tokens->str);
				free(vars);
				return ;
			}
			super_concat(&vars, "\n");
			super_concat(&vars, cmd->tokens->str);
			cmd->tokens = cmd->tokens->next;
		}
		print_io(cmd, vars);
		free(vars);
	}
}
