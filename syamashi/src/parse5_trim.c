/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5_trim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 16:57:59 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/10 16:06:12 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

/*    prev    mov      mov->n  mov->n->n
**    [space] [str]    [str]   [space]
** -> [space] [strstr] [space]
**
**    prev    mov(*ps) mov->n
**    NULL    [str]    [str]
** -> NULL    [strstr]
**
**  when pre_type is dir, ESC keeps left.
**  case [ > \$test ] -> [ > $test]
**  case [ >  $test ] -> [ > env]
*/
static void	get_info(char **line, int *type, const t_list *mov)
{
	*line = ((t_pack *)(mov)->content)->line;
	*type = ((t_pack *)(mov)->content)->type;
}

void	pack_marge
(t_list **prev, t_list **mov, t_list **packs, const int pre_type)
{
	t_pack	*pack;
	t_list	*new;
	char	*line;
	int		type;

	new_pack(&pack);
	get_info(&line, &type, *mov);
	if (is_dir(pre_type) && type == ESC)
		pack_join(&pack, "\\", 1);
	pack_join(&pack, line, ft_strlen(line));
	get_info(&line, &type, (*mov)->next);
	if (is_dir(pre_type) && type == ESC)
		pack_join(&pack, "\\", 1);
	pack_join(&pack, line, ft_strlen(line));
	if (!(new = ft_lstnew(pack)))
		exit(ft_error("", 1));
	new->next = (*mov)->next->next;
	if (*prev)
		(*prev)->next = new;
	else
		*packs = new;
	ft_lstdelone((*mov)->next, pack_free);
	ft_lstdelone(*mov, pack_free);
	*mov = new;
}

void	pack_del(t_list **prev, t_list **mov, t_list **packs)
{
	t_list	*tmp;
	
	tmp = (*mov)->next;
	if (*prev)
		(*prev)->next = (*mov)->next;
	else
		*packs = (*mov)->next;
	ft_lstdelone(*mov, pack_free);
	*mov = tmp;
}

void	quote_del(t_list **packs)
{
	t_list	*mov;
	t_list	*prev;
	int		type;
	bool	dir_flag;

	prev = NULL;
	mov = *packs;
	dir_flag = false;
	while (mov)
	{
		type = ((t_pack *)mov->content)->type;
		if (is_dir(type))
		{
			dir_flag = true;
			mov = mov->next;
		}
		else if (dir_flag && type == SPACE)
			dir_flag = false;
		if (!dir_flag && (type == SQUOTE || type == DQUOTE))
			pack_del(&prev, &mov, packs);
		else
		{
			prev = mov;
			mov = mov->next;
		}
	}
}

int		is_join(int type)
{
	return (type == STR ||
			type == SSTR ||
			type == ESC ||
			type == SQUOTE ||
			type == DQUOTE);
}

void	strs_join(t_list **packs)
{
	t_list	*mov;
	t_list	*prev;
	int		type;
	int		ntype;
	int		pre_type;

	prev = NULL;
	mov = *packs;
	pre_type = -1;
	while (mov)
	{
		type = ((t_pack *)mov->content)->type;
		if (is_join(type) && mov->next)
		{
			ntype = ((t_pack *)mov->next->content)->type;
			if (is_join(ntype))
			{
				pack_marge(&prev, &mov, packs, pre_type);
				continue;
			}
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

void	packs_trim(t_list **packs)
{
	int		type;
	int		nexttype;
	t_list	*mov;

	quote_del(packs);
	strs_join(packs);
	space_del(packs);
}
