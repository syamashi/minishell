/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsenew3_storediv.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 19:42:58 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/08 02:33:56 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	store_div(t_list **store)
{
	t_list	*top;
	t_list	*packs;
	t_list	*prev;
	t_list	*new;
	char	*line;
	int		type;

	top = *store;
	packs = top->content; // list
	prev = NULL;
	while (packs)
	{
		line = ((t_pack *)packs->content)->line;
		type = ((t_pack *)packs->content)->type;
		if (type == SCOLON)
		{
			if (packs->next)
			{
				if (!(new = ft_lstnew(packs->next)))
					exit(ft_error("minishell: malloc failed", 1));
				ft_lstadd_back(&top, new);
				top = top->next;
			}
			pack_free(packs->content);
			free(packs);
			packs = top->content;
			if (prev)
				prev->next = NULL;
			prev = NULL;
		}
		else
		{
			prev = packs;
			packs = packs->next;
		}
	}
}
