/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 08:12:08 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/18 08:12:10 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_print_unsigned(unsigned int i, int *len)
{
	if (i >= 10)
		ft_print_unsigned(i / 10, len);
	ft_putchar((i % 10) + '0', len);
}

static void	ft_deal_arg(char c, va_list *arg, int	*len)
{
	if (c == 'c')
		ft_putchar(va_arg(*arg, int), len);
	else if (c == 's')
		ft_putstr(va_arg(*arg, char *), len);
	else if (c == 'p')
		ft_to_hexa(va_arg(*arg, size_t), "0123456789abcdef", len);
	else if (c == 'd')
		ft_putnbr(va_arg(*arg, int), len);
	else if (c == 'i')
		ft_putnbr(va_arg(*arg, int), len);
	else if (c == 'u')
		ft_print_unsigned(va_arg(*arg, unsigned int), len);
	else if (c == 'x')
		ft_putnbr_hexa(va_arg(*arg, unsigned int), "0123456789abcdef", len);
	else if (c == 'X')
		ft_putnbr_hexa(va_arg(*arg, unsigned int), "0123456789ABCDEF", len);
	else if (c == '%')
		ft_putchar('\%', len);
}

int	ft_printf(const char *s, ...)
{
	va_list	args;
	int		i;
	int		len;

	i = 0;
	len = 0;
	va_start(args, s);
	while (s[i])
	{
		if (s[i] == '%' && s[i + 1] != '\0')
		{
			ft_deal_arg(s[i + 1], &args, &len);
			i++;
		}
		else
			ft_putchar(s[i], &len);
		i++;
	}
	va_end(args);
	return (len);
}
