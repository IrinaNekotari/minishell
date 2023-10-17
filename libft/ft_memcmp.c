/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:02:25 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 10:02:27 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	char	*cast1;
	char	*cast2;

	cast1 = (char *)s1;
	cast2 = (char *)s2;
	while (n > 0)
	{
		if (*cast1 != *cast2)
			return ((unsigned char)*cast1 - (unsigned char)*cast2);
		if (n)
		{
			++cast1;
			++cast2;
		}
		n--;
	}
	return (0);
}
