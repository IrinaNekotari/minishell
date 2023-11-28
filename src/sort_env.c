/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:37:48 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/21 11:52:10 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
On a pas besoin de ce fichier : Size env existe déjà dans chains/length ou depths, strcmp existe déjà (et utilise mon ft_equals ...)
Essaye de tout mettre dans export
*/
static int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

size_t			size_env(t_env *lst)
{
	size_t	lst_len;

	lst_len = 0;
	while (lst && lst->next != NULL)
	{
		if (lst->value != NULL)
		{
			lst_len += ft_strlen(lst->value);
			lst_len++;
		}
		lst = lst->next;
	}
	return (lst_len);
}

int	str_env_len(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

char	*env_to_str(t_env *lst)
{
	char	*env;
	int		i;
	int		j;

	if (!(env = malloc(sizeof(char) * 10 * size_env(lst) + 1)))
		return (NULL);
	i = 0;
	while (lst && lst->next != NULL)
	{
		if (lst->name != NULL)
		{
			j = 0;
			while (lst->name[j])
			{
				env[i] = lst->name[j];
				i++;
				j++;
			}
		}
		env[i] = '=';
		i++;
		if (lst->value != NULL)
		{
			j = 0;
			while (lst->value[j])
			{
				env[i] = lst->value[j];
				i++;
				j++;
			}
		}
		if (lst->next->next != NULL)
			env[i++] = '\n';
		lst = lst->next;
	}
	env[i] = '\0';
	return (env);
}

void	sort_env(char **tab, int env_len)
{
	int	ordered;
	int	i;
	char		*tmp;

	ordered = 0;
	while (tab && ordered == 0)
	{
		ordered = 1;
		i = 0;
		while (i < env_len - 1)
		{
			if (ft_strcmp(tab[i], tab[i + 1]) > 0)
			{
				tmp = tab[i];
				tab[i] = tab[i + 1];
				tab[i + 1] = tmp;
				ordered = 0;
			}
			i++;
		}
		env_len--;
	}
}

char	*print_sorted_env(t_env *env)
{
	int	i;
	char	*str_env;
	char	*to_print;
	char		**tab;

	str_env = env_to_str(env);
	tab = ft_split(str_env, '\n');
	free(str_env);
	sort_env(tab, str_env_len(tab));
	to_print = ft_calloc(1, sizeof(char));
	i = 0;
	while (tab[i])
	{	
		super_concat(&to_print, "declare -x ");
		super_concat(&to_print, tab[i]);
		super_concat(&to_print, "\n");
		i++;
	}
	free_liste(tab);
	return (to_print);
}
