/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_envexpand3_repack.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 14:31:02 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/17 19:29:06 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

t_list	*space_strtoken(char *line)
{
	t_token	t;
	t_list	*list;
	t_pack	*pack;

	def_strtoken(&t, &list, &line, &pack);
	while (line[++t.i])
	{
		if (is_space(line[t.i]))
			pack_metaadd(&pack, &list, " ", SPACE);
		while (is_space(line[t.i]) && line[t.i])
			t.i++;
		t.j = t.i;
		while (!is_space(line[t.i]) && line[t.i])
			t.i++;
		if (t.j != t.i)
			pack_join(&pack, line + t.j, t.i - t.j);
		if (*pack->line)
			pack_add(&pack, &list, STR);
		--t.i;
	}
	if (t.i == 0)
		pack_add(&pack, &list, STR);
	return (pack_end(&pack, &list));
}

void	repack(t_list **prev, t_list **mov, t_list **packs)
{
	t_list	*new;
	t_list	*newlast;
	char	*line;

	line = ((t_pack *)(*mov)->content)->line;
	new = space_strtoken(line);
	newlast = ft_lstlast(new);
	if (*prev)
		(*prev)->next = new;
	else
		*packs = new;
	ft_lstadd_back(&new, (*mov)->next);
	ft_lstdelone(*mov, pack_free);
	if (newlast)
		*mov = newlast;
	else
		*mov = new;
}
