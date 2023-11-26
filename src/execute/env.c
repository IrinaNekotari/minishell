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

int		is_valid_env(const char *env)
{
	int		i;

	i = 0;
	if (ft_isdigit(env[i]) == 1)
		return (0);
	while (env[i] && env[i] != '=')
	{
		if (ft_isalnum(env[i]) == 0)
			return (-1);
		i++;
	}
	if (env[i] != '=')
		return (2);
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
		print_io(cmd, vars, &main);
		free(vars);
	}
}
