/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:01:59 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 10:02:00 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*castd;
	unsigned char	*casts;

	castd = (unsigned char *)dest;
	casts = (unsigned char *)src;
	if (dest == src)
		return (dest);
	if (castd < casts)
		while (n--)
			*castd++ = *casts++;
	else
	{
		castd += n;
		casts += n;
		while (n--)
			*--castd = *--casts;
	}
	return (dest);
}
