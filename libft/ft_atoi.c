/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 10:30:02 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/12 10:30:03 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_power(int pow)
{
	int	i;
	int	resultat;

	i = 0;
	resultat = 1;
	while (i < pow - 1)
	{
		resultat *= 10;
		i++;
	}
	return (resultat);
}

static int	ft_to_string(const char *str, int i)
{
	int	resultat;
	int	u;
	int	v;

	u = 0;
	v = i;
	resultat = 0;
	while (str[v] >= '0' && str[v++] <= '9')
	{
		u++;
	}
	while (u > 0)
	{
		resultat += ft_power(u--) * (str[i++] - 48);
	}
	return (resultat);
}

static int	ft_phase_two(const char *str, int i)
{
	int	signe;
	int	resultat;
	int	boule;

	signe = 1;
	resultat = 0;
	boule = 0;
	while (str[i])
	{
		if (str[i] == '-' && boule == 0)
			signe = -1;
		if ((str[i] == '-' || str[i] == '+') && boule == 0)
			boule = 1;
		else if ((str[i] == '-' || str[i] == '+') && boule == 1)
			return (0);
		else if (str[i] >= '0' && str[i] <= '9')
		{
			resultat = ft_to_string(str, i);
			break ;
		}
		else
			return (0);
		i++;
	}
	return (resultat * signe);
}

int	ft_atoi(const char *nptr)
{
	int	i;

	i = 0;
	while (nptr[i])
	{
		if (nptr[i] == '-' || nptr[i] == '+')
			return (ft_phase_two(nptr, i));
		else if (nptr[i] >= '0' && nptr[i] <= '9')
			return (ft_phase_two(nptr, i));
		else if (nptr[i] != 32 && nptr[i] != '\f' && nptr[i] != '\n'
			&& nptr[i] != '\r' && nptr[i] != '\t' && nptr[i] != '\v')
			return (0);
		i++;
	}
	return (0);
}
