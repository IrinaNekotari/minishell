#include "minishell.h"


int		ft_env(t_env *env)
{
	while (env && env->next != NULL)
	{
		ft_putendl(env->val);
		env = env->next;
	}
	if (env)
		ft_putendl(env->val);
	return (SUCCESS);
}

static size_t	env_size(char *env)
{
	size_t		i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

void	*ft_memdel(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (NULL);
}

static void		free_env(t_main *shell, t_env *env)
{
	if (shell->env == env && env->next == NULL)
	{
		ft_memdel(shell->env->val);
		shell->env->val = NULL;
		shell->env->next = NULL;
		return ;
	}
	ft_memdel(env->val);
	ft_memdel(env);
}

int				ft_unset(t_cmd *cmd, t_main *shell)
{
	t_env	*env;
	t_env	*tmp;

	env = shell->env;
	if (!(cmd->tokens->str))
		return (SUCCESS);
	if (ft_strncmp(cmd->tokens->str, env->val, env_size(env->val)) == 0)
	{
		shell->env = (env->next) ? env->next : shell->env;
		free_env(shell, env);
		return (SUCCESS);
	}
	while (env && env->next)
	{
		if (ft_strncmp(cmd->tokens->str, env->next->val, env_size(env->next->val)) == 0)
		{
			tmp = env->next->next;
			free_node(shell, env->next);
			env->next = tmp;
			return (SUCCESS);
		}
		env = env->next;
	}
	return (SUCCESS);
}