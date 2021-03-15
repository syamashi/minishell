/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_storediv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 19:42:58 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 19:22:51 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	scolon_del(t_list **packs, t_command **top, t_list **prev, int type)
{
	t_command	*new;

	if ((*packs)->next)
	{
		if (!(new = ft_clstnew((*packs)->next)))
			exit(ft_error("malloc failed", 1, STDERR));
		new->and_or = (type == DPIPE || type == DAND) ? type : 0;
		ft_clstadd_back(top, new);
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

void	store_div(t_command **store)
{
	t_command	*top;
	t_list		*packs;
	t_list		*prev;
	char		*line;
	int			type;

	top = *store;
	packs = top->content;
	prev = NULL;
	while (packs)
	{
		line = ((t_pack *)packs->content)->line;
		type = ((t_pack *)packs->content)->type;
		if (type == SCOLON || type == DPIPE || type == DAND)
			scolon_del(&packs, &top, &prev, type);
		else
		{
			prev = packs;
			packs = packs->next;
		}
	}
}
