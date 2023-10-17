/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:01:46 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 10:01:47 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*castdest;
	const char	*castsrc;

	if (!dest && !src)
		return (NULL);
	castdest = (char *) dest;
	castsrc = (const char *) src;
	while (n > 0)
	{
		*castdest = *castsrc;
		n--;
		++castdest;
		++castsrc;
	}
	return (dest);
}
