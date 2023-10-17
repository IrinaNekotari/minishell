/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 11:08:11 by nmascrie          #+#    #+#             */
/*   Updated: 2023/04/19 11:08:12 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*ret;
	t_list	*back;

	if (!f || !del)
		return (NULL);
	ret = NULL;
	while (lst)
	{
		back = ft_lstnew((*f)(lst->content));
		if (!back)
		{
			while (ret)
			{
				back = ret->next;
				(*del)(ret->content);
				free(ret);
				ret = back;
			}
			lst = NULL;
			return (NULL);
		}
		ft_lstadd_back(&ret, back);
		lst = lst->next;
	}
	return (ret);
}
