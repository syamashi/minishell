/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_storediv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 19:42:58 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/12 14:16:59 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	scolon_del(t_list **packs, t_list **top, t_list **prev)
{
	t_list	*new;

	if ((*packs)->next)
	{
		if (!(new = ft_lstnew((*packs)->next)))
			exit(ft_error("minishell: malloc failed", 1));
		ft_lstadd_back(top, new);
		*top = (*top)->next;
	}
	ft_lstdelone(*packs, pack_free);
	*packs = (*top)->content;
	if (*prev)
		(*prev)->next = NULL;
	*prev = NULL;
}

/*
**            semi               semi
** store[0]     -     store[1]     -     store[2]
**           CMD
**  -packs[0] - packs[1] -
**   -line       -line
**   -type       -type
*/

void	store_div(t_list **store)
{
	t_list	*top;
	t_list	*packs;
	t_list	*prev;
	char	*line;
	int		type;

	top = *store;
	packs = top->content;
	prev = NULL;
	while (packs)
	{
		line = ((t_pack *)packs->content)->line;
		type = ((t_pack *)packs->content)->type;
		if (type == SCOLON)
			scolon_del(&packs, &top, &prev);
		else
		{
			prev = packs;
			packs = packs->next;
		}
	}
}
