/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 11:07:23 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 11:07:24 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*to_del;

	if (lst && del && *lst)
	{
		while (lst && *lst)
		{
			to_del = (*lst)->next;
			ft_lstdelone(*lst, del);
			*lst = to_del;
		}
	}
}
