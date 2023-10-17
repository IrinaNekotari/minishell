/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 08:26:38 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/18 08:26:39 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar(char c, int *len)
{
	write(1, &c, 1);
	(*len)++;
}

void	ft_putstr(char *str, int *len)
{
	unsigned int	i;

	i = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		(*len) += 6;
	}
	else
	{
		while (str[i])
		{
			ft_putchar(str[i], len);
			i++;
		}
	}
}

void	ft_to_hexa(size_t ptr, char *set, int *len)
{
	size_t		quot;
	int			i;
	char		*ret;

	quot = ptr;
	ret = malloc(16 * sizeof(char));
	i = 0;
	if (!ptr)
		ft_putstr("(nil)", len);
	else
	{
		ft_putstr("0x", len);
		while (quot != 0)
		{
			ret[i] = set[quot % 16];
			quot /= 16;
			i++;
		}
		while (i--)
			ft_putchar(ret[i], len);
	}
	free(ret);
}

void	ft_putnbr_hexa(unsigned int nb, char	*set, int *len)
{	
	if (nb >= 16)
	{
		ft_putnbr_hexa(nb / 16, set, len);
		ft_putnbr_hexa(nb % 16, set, len);
	}
	if (nb < 16)
	{
		ft_putchar(set[nb], len);
	}
}

void	ft_putnbr(int nb, int *len)
{
	if (nb < -2147483647)
	{
		ft_putchar('-', len);
		ft_putchar('2', len);
		nb = 147483648;
	}
	if (nb > 2147483646)
	{
		ft_putchar('2', len);
		nb = 147483647;
	}
	if (nb < 0)
	{
		ft_putchar('-', len);
		nb = -nb;
	}
	if (nb >= 10)
	{
		ft_putnbr(nb / 10, len);
		ft_putnbr(nb % 10, len);
	}
	if (nb < 10)
	{
		ft_putchar(nb + '0', len);
	}
}
