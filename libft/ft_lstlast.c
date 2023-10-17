/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 11:08:17 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 11:08:18 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (NULL == lst)
		return (NULL);
	else if (NULL == lst->content)
		return (lst);
	else if (NULL == lst->next)
		return (lst);
	else
		return (ft_lstlast(lst->next));
}
