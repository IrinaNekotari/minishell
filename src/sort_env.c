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
On a pas besoin de ce fichier : Size env 
existe déjà dans chains/length ou depths, 
strcmp existe déjà (et utilise mon ft_equals ...)
Essaye de tout mettre dans export
*/
static int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

size_t	size_env(t_env *lst)
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

void	sort_env(char **tab)
{
	int		sorted;
	int		i;
	char	*buffer;

	sorted = 1;
	i = 1;
	while (tab[i])
	{
		if (ft_strcmp(tab[i], tab[i - 1]) < 0)
		{
			buffer = ft_strdup(tab[i]);
			free(tab[i]);
			tab[i] = ft_strdup(tab[i - 1]);
			free(tab[i - 1]);
			tab[i - 1] = ft_strdup(buffer);
			free(buffer);
			sorted = 0;
		}
		i++;
	}
	if (!sorted)
		sort_env(tab);
}

char	*print_sorted_env(t_env *env)
{
	char		*to_print;
	char		**tab;
	int			i;

	tab = env_to_array(env);
	sort_env(tab);
	to_print = ft_calloc(1, sizeof(char));
	i = 0;
	while (tab[i])
	{
		super_concat(&to_print, "declare -x ");
		super_concat(&to_print, tab[i]);
		i++;
	}
	free_liste(tab);
	return (to_print);
}
