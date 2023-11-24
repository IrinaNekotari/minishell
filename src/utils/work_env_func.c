/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_env_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 09:42:48 by nmascrie          #+#    #+#             */
/*   Updated: 2023/11/21 09:42:51 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    update_env(t_env **env, char *name, char *newvalue)
{
	while ((*env))
	{
		if (ft_equals((*env)->name, name))
		{
			free((*env)->value);
			(*env)->value = ft_strdup(newvalue);
			rollback_env(env);
			return ;
		}
		if ((*env)->next)
			(*env) = (*env)->next;
		else
			break;
	}
	rollback_env(env);
}

void    del_from_env(t_env **env, char *name)
{
	t_env	*element;

	while ((*env))
	{
		if (ft_equals((*env)->name, name))
		{
			element = (*env);
			free(element->name);
			free(element->value);
			if (element->next)
			{
				(*env) = (*env)->next;
				if (element->previous)
				{
					(*env)->previous = element->previous;
					element->previous->next = element->next;
				}
				else
					(*env)->previous = NULL;
			}
			else if (element->previous)
			{
				(*env) = (*env)->previous;
				(*env)->next = NULL;
			}
			free(element);
		}
		if ((*env)->next)
			(*env) = (*env)->next;
		else
			break;
	}
	rollback_env(env);
}

void    add_to_env(t_env **env, char *name, char *value)
{
    while ((*env))
    {
        if (ft_equals((*env)->name, name))
        {
          free((*env)->value);
          (*env)->value = ft_strdup(value);
          rollback_env(env);
          return ;
        }
        if ((*env)->next)
        	(*env) = (*env)->next;
        else
        	break;
    }
    (*env)->next = ft_calloc(1, sizeof(t_env));
    (*env)->next->previous = (*env);
    (*env) = (*env)->next;
    (*env)->name = ft_strdup(name);
    (*env)->value = ft_strdup(value);
    (*env)->next = NULL;
    rollback_env(env);
}

char	*ft_getenv(t_env *env, char *search)
{
	char	*ret;

	if (!search)
		return (NULL);
	while (env)
	{
		if (ft_equals(env->name, search))
		{
			ret = ft_strdup(env->value);
			while (env->previous)
				env = env->previous;
			return (ret);
		}
		env = env->next;
	}
	return (NULL);
}
