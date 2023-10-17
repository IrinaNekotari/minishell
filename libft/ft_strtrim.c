/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:06:19 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 10:06:21 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	checkset(char c, char const *set)
{
	unsigned int	i;

	i = 0;
	while (set[i])
		if ((char) set[i++] == c)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	i;
	size_t	debut;
	size_t	fin;

	i = 0;
	debut = 0;
	fin = ft_strlen(s1);
	while (s1[debut] && checkset(s1[debut], set))
		debut++;
	while (fin > debut && checkset(s1[fin - 1], set))
		fin--;
	str = (char *) malloc(sizeof(*s1) * (fin - debut + 1));
	if (!str)
		return (NULL);
	while (debut < fin)
		str[i++] = s1[debut++];
	str[i] = 0;
	return (str);
}
