#include "minishell.h"

//PS ; Je les ai mises dans work_env_func


/**
* Retourne la profondeur de l'environnement
*/
static int	env_depth(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	while (env->previous)
		env = env->previous;
	return (i);
}

/**
* Retourne la plus longue taille de chaine de l'environnement
*/
static int	env_longest(t_env *env)
{
	int	len;
	int	bck;

	len = 0;
	while (env)
	{
		bck = (int)ft_strlen(env->name) + (int)ft_strlen(env->value);
		if (bck > len)
			len = bck;
		env = env->next;
	}
	while (env->previous)
		env = env->previous;
	return (len);
}

/**
* Crée un array a partir des variables d'environnement
*/
char	**env_to_array(t_env *env)
{
	char	**ret;
	int			len;
	int			i;

	len = env_longest(env) + 3;
	i = 0;
	ret = ft_calloc((env_depth(env) + 1) * len, sizeof(char));
	while (env)
	{
		ret[i] = ft_strdup(env->name);
		super_concat(&ret[i], "=");
		super_concat(&ret[i], env->value);
		super_concat(&ret[i], "\n");
		env = env->next;
		i++;	
	}
	while (env->previous)
		env = env->previous;
	ret[i] = NULL;
	return (ret);
}

static char	*generate_env(char *env, int meth)
{
	int	i;
	int	j;
	char	*a;
	char	*b;

	i = -1;
	j = -1;
	a = ft_calloc(ft_strlen(env), sizeof(char));
	b = ft_calloc(ft_strlen(env), sizeof(char));
	while (env[++i] && env[i] != '=')
		a[i] = env[i];
	i++;
	while (env[i])
		b[++j] = env[i++];
	if (meth == 1)
	{
		free(b);
		return (a);
	}
	else
	{
		free(a);
		return (b);
	}
}

t_env	*make_env(char **env)
{
	int		i;
	t_env	*ret;

	i = 1;
	ret = ft_calloc(1, sizeof(t_env));
	ret->previous = NULL;
	ret->name = generate_env(env[0], 1);
	ret->value = generate_env(env[0], 2);
	while (env[i])
	{
		ret->next = ft_calloc(1, sizeof(t_env));
		ret->next->previous = ret;
		ret = ret->next;
		ret->name = generate_env(env[i], 1);
		ret->value = generate_env(env[i], 2);
		ret->next = NULL;
		i++;
	}
	while (ret->previous)
		ret = ret->previous;
	return (ret);
}
