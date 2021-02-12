/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5_trim2_quotedel.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 15:12:42 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/12 18:51:24 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

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

/*
**    [space] [str] [str] [space]
** -> [space] [strstr] [space]
**
**    NULL [str] [str]
** -> NULL [strstr]
**
**  when pre_type is dir, ESC keeps left.
**  [ > \$test ] -> [ > $test]
**  [ > $test ] -> [ > env]
*/

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
		if (is_dir(type) && (dir_flag = true))
			mov = mov->next;
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
