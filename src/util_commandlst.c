/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_commandlst.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 21:29:58 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/17 21:33:19 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

t_command	*ft_clstnew(void *content)
{
	t_command	*list;

	if (!(list = (t_command *)malloc(sizeof(t_command))))
		return (NULL);
	list->content = content;
	list->next = NULL;
	list->and_or = 0;
	return (list);
}

void	ft_clstdelone(t_command *lst, void (*del)(void*))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
	lst = NULL;
}

void	ft_clstadd_back(t_command **lst, t_command *new)
{
	t_command	*mov;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	mov = *lst;
	while (mov->next)
		mov = mov->next;
	mov->next = new;
}
