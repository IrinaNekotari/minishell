/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:02:11 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 10:02:12 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*cast;
	unsigned int	i;

	cast = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (cast[i] == (unsigned char)c)
			return ((char *)s + i);
		i++;
	}
	return (NULL);
}
