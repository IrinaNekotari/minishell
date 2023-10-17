/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:02:37 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 10:02:38 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = src[i];
	return (dest);
}

char	*ft_strdup(const char *s)
{
	int		i;
	char	*copie;

	copie = NULL;
	i = ft_strlen(s);
	copie = (char *) malloc((i + 1) * sizeof(char));
	if (!copie)
		return (NULL);
	ft_strcpy(copie, s);
	return (copie);
}
