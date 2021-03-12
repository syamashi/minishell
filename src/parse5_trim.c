/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5_trim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 16:57:59 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/12 20:40:01 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

int		is_strsjoin(int type)
{
	return (type == STR ||
			type == SSTR ||
			type == ESC ||
			type == SQUOTE ||
			type == DQUOTE);
}

void	pack_marge(
	t_list **prev, t_list **mov, t_list **packs, const int pre_type)
{
	t_pack	*pack;
	t_list	*new;
	char	*line;
	int		type;

	new_pack(&pack);
	packinfo_get(&line, &type, *mov);
	if (is_dir(pre_type) && type == ESC)
		pack_join(&pack, "\\", 1);
	pack_join(&pack, line, ft_strlen(line));
	packinfo_get(&line, &type, (*mov)->next);
	if (is_dir(pre_type) && type == ESC)
		pack_join(&pack, "\\", 1);
	pack_join(&pack, line, ft_strlen(line));
	if (!(new = ft_lstnew(pack)))
		exit(ft_error("malloc failed", 1));
	new->next = (*mov)->next->next;
	if (*prev)
		(*prev)->next = new;
	else
		*packs = new;
	ft_lstdelone((*mov)->next, pack_free);
	ft_lstdelone(*mov, pack_free);
	*mov = new;
}

void	strs_join(t_list **packs)
{
	t_list	*mov;
	t_list	*prev;
	int		type;
	int		pre_type;

	prev = NULL;
	mov = *packs;
	pre_type = -1;
	while (mov)
	{
		type = ((t_pack *)mov->content)->type;
		if (is_strsjoin(type) && mov->next)
			if (is_strsjoin(((t_pack *)mov->next->content)->type))
			{
				pack_marge(&prev, &mov, packs, pre_type);
				continue;
			}
		prev = mov;
		if (type != SPACE)
			pre_type = type;
		mov = mov->next;
	}
}

void	space_del(t_list **packs)
{
	t_list	*mov;
	t_list	*prev;
	int		type;

	prev = NULL;
	mov = *packs;
	while (mov)
	{
		type = ((t_pack *)mov->content)->type;
		if (type == SPACE)
			pack_del(&prev, &mov, packs);
		else
		{
			prev = mov;
			mov = mov->next;
		}
	}
}

/*
**	types = {STR, DIRS, SPACE, QUOTES, PIPE, SSTR}
**  after DIRS, NOT del QUOTES
*/
void	null_del(t_list **packs)
{
	t_list	*mov;
	t_list	*prev;
	int		type;
	int		pretype;
	char	*line;

	prev = NULL;
	pretype = STR;
	mov = *packs;
	while (mov)
	{
		type = ((t_pack *)mov->content)->type;
		line = ((t_pack *)mov->content)->line;
		if (type == STR && !is_quote(pretype) && !*line)
			pack_del(&prev, &mov, packs);
		else
		{
			prev = mov;
			mov = mov->next;
		}
		pretype = type;
	}
}

void	packs_trim(t_list **packs)
{
	int		type;
	int		nexttype;
	t_list	*mov;

	null_del(packs);
	quote_del(packs);
	strs_join(packs);
	space_del(packs);
}
