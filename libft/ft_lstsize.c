/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 11:07:56 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 11:07:58 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	if (NULL == lst || NULL == lst->content)
		return (0);
	else if (NULL == lst->next)
		return (1);
	else
		return (1 + ft_lstsize(lst->next));
}
