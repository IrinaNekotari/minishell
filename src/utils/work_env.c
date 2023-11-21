/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 09:42:33 by nmascrie          #+#    #+#             */
/*   Updated: 2023/11/21 09:42:42 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		i++;
		if (env->next)
			env = env->next;
		else
			break ;
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
	bck = 0;
	while (env)
	{
		bck = (int)ft_strlen(env->name) + (int)ft_strlen(env->value);
		if (bck > len)
			len = bck;
		if (env->next)
			env = env->next;
		else
			break ;
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
		i++;
		if (env->next)
			env = env->next;
		else
			break ;
	}
	while (env->previous)
		env = env->previous;
	ret[i] = NULL;
	return (ret);
}

void	generate_env(char *env, char **name, char **value)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	(*name) = ft_calloc(ft_strlen(env), sizeof(char));
	(*value) = ft_calloc(ft_strlen(env), sizeof(char));
	while (env[++i] && env[i] != '=')
		(*name)[i] = env[i];
	i++;
	while (env[i])
		(*value)[++j] = env[i++];
}

t_env	*make_env(char **env)
{
	int		i;
	t_env	*ret;

	i = 1;
	ret = ft_calloc(1, sizeof(t_env));
	ret->previous = NULL;
	generate_env(env[0], &ret->name, &ret->value);
	while (env[i])
	{
		ret->next = ft_calloc(1, sizeof(t_env));
		ret->next->previous = ret;
		ret = ret->next;
		generate_env(env[i], &ret->name, &ret->value);
		ret->next = NULL;
		i++;
	}
	while (ret->previous)
		ret = ret->previous;
	return (ret);
}
