#include "minishell.h"

static void	super_concat(char **a, char **b)
{
	char	*bck;

	bck = ft_concat((*a), (*b));
	free((*a));
	(*a) = ft_strdup(bck);
	free(bck);
}

/**
* Transforme un char** en char*
*/
static char	*flatten(char **s)
{
	int		i;
	char	*ret;

	i = 1;
	if (!s || !s[0])
		return (NULL);
	if (!s[1])
		return (ft_strdup(s[0]));
	else
	{
		ret = ft_strdup(s[0]);
		while (s[i])
		{
			super_concat(&ret, &s[i]);
			i++;
		}
	}
	return (ret);
}

static char	**copy_env(char **env)
{
	char	**ret;
	int			i;

	i = 0;
	while (env[i])
		i++;
	ret = ft_calloc(i*100, sizeof(char));
	ret[i--] = NULL;
	while (i >= 0)
	{
		ret[i] = ft_strdup(env[i]);
		i--;
	}
	return (ret);
}

static void	append_env(char ***env, char *s)
{
	char	**ret;
	int			i;

	i = 0;
	while ((*env)[i])
		i++;
	ret = ft_calloc(i*100 + 2, sizeof(char));
	ret[i] = ft_strdup(s);
	ret[i-- + 1] = NULL;
	while (i >= 0)
	{
		ret[i] = ft_strdup((*env)[i]);
		i--;
	}
	free_liste((*env));
	(*env) = copy_env(ret);
	free_liste(ret);
}

/**
* NB : env imprime toutes les variables d'environnement sans arguments
* avec un argument, elle fait pareil mais en ajoutant une nouvelle ligne
* l'argument doit être de forme A=B sinon elle est pas contente
* TODO : A norminer
*/
void	ft_env(t_cmd *cmd, char **env)
{
	char	*vars;
	char	**cpy;

	if (!cmd->tokens->next->str)
	{
		vars = flatten(env);
		print_io(cmd, vars);
		free(vars);
	}
	else
	{
		cmd->tokens = cmd->tokens->next;
		cpy = copy_env(env);
		while (cmd->tokens->str)
		{
			if(!ft_strchr(cmd->tokens->str, '='))
			{
				handle_output_create(cmd);
				ft_printf("[\x1b[31mENV ERROR\x1b[37m] : \"%s\" is not a file or a directory !", cmd->tokens->str);
				free_liste(cpy);
				return ;
			}
			else
				append_env(&cpy, cmd->tokens->str);
			cmd->tokens = cmd->tokens->next;
		}
		vars = flatten(cpy);
		print_io(cmd, vars);
		free(vars);
		free_liste(cpy);
	}
}
