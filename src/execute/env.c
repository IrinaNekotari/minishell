#include "minishell.h"

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

static int	err_env(t_cmd *cmd, char *vars)
{
	if(!ft_strchr(cmd->tokens->str, '='))
	{
		handle_output_create(cmd);
		error_print(ERROR, "No such file or directory",
				cmd->tokens->str);
		free(vars);
		return (0);
	}
	return (1);
}

void	ft_env(t_cmd *cmd, t_main *main)
{
	char	*vars;

	if (!cmd->tokens->next->str)
	{
		vars = create_list(main->env);
		print_io(cmd, vars, &main);
		free(vars);
	}
	else
	{
		vars = create_list(main->env);
		cmd->tokens = cmd->tokens->next;
		while (cmd->tokens->str)
		{
			if (!err_env(cmd, vars))
				return ;
			super_concat(&vars, "\n");
			super_concat(&vars, cmd->tokens->str);
			cmd->tokens = cmd->tokens->next;
		}
		print_io(cmd, vars, &main);
		free(vars);
	}
}
