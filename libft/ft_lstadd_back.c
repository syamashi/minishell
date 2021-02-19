/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 01:47:40 by syamashi          #+#    #+#             */
/*   Updated: 2020/12/16 01:26:56 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*endlst;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	endlst = ft_lstlast(*lst);
	endlst->next = new;
}
