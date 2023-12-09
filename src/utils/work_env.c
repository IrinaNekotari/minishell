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

void	init_block_env(t_env **env, char *name, char *value)
{
	(*env) = ft_calloc(1, sizeof(t_env));
	(*env)->previous = NULL;
	(*env)->next = NULL;
	(*env)->name = ft_strdup(name);
	(*env)->value = ft_strdup(value);
}

void	add_env_block(t_env **env, char *name, char *value)
{
	if (!env || !(*env))
		init_block_env(env, name, value);
	else
	{
		(*env)->next = ft_calloc(1, sizeof(t_env));
		(*env)->next->previous = (*env);
		(*env) = (*env)->next;
		(*env)->next = NULL;
		(*env)->name = ft_strdup(name);
		(*env)->value = ft_strdup(value);
	}
}

/**
* Crée un array a partir des variables d'environnement
*/
char	**env_to_array(t_env *env)
{
	char	**ret;
	int		len;
	int		i;

	len = env_length(env) + 3;
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
	if (!env[i])
	{
		free((*value));
		(*value) = NULL;
		return ;
	}
	i++;
	while (env[i])
		(*value)[++j] = env[i++];
}

t_env	*make_env(char **env)
{
	int		i;
	t_env	*ret;
	char	*name;
	char	*value;

	i = 0;
	ret = NULL;
	while (env[i])
	{
		generate_env(env[i], &name, &value);
		add_env_block(&ret, name, value);
		free(name);
		free(value);
		i++;
	}
	while (ret->previous)
		ret = ret->previous;
	return (ret);
}
