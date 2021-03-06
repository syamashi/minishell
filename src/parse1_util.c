/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:50:00 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 19:22:51 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	new_pack(t_pack **pack)
{
	if (!(*pack = (t_pack *)malloc(sizeof(t_pack))))
		exit(ft_error("malloc failed", 1, STDERR));
	if (!((*pack)->line = ft_strdup("")))
		exit(ft_error("malloc failed", 1, STDERR));
	(*pack)->type = STR;
}

void	pack_join(t_pack **pack, char *str, int len)
{
	char *tmp;
	char *add;

	tmp = (*pack)->line;
	if (!(add = ft_substr(str, 0, len)))
		exit(ft_error("malloc failed", 1, STDERR));
	if (!((*pack)->line = ft_strjoin((*pack)->line, add)))
		exit(ft_error("malloc failed", 1, STDERR));
	free(tmp);
	tmp = NULL;
	free(add);
	add = NULL;
}

void	pack_add(t_pack **pack, t_list **list, int type)
{
	t_list *new;

	(*pack)->type = type;
	if (!(new = ft_lstnew(*pack)))
		exit(ft_error("malloc failed", 1, STDERR));
	ft_lstadd_back(list, new);
	new_pack(pack);
}

void	pack_metaadd(t_pack **pack, t_list **list, char *str, int type)
{
	t_list	*new;

	if (type == SPACE)
	{
		new = ft_lstlast(*list);
		if (new && ((t_pack *)new->content)->type == SPACE)
			return ;
	}
	pack_join(pack, str, ft_strlen(str));
	pack_add(pack, list, type);
}

void	pack_stradd(t_pack **pack, t_list **list, t_token *t)
{
	if (t->j != t->i)
	{
		pack_join(pack, t->line + t->j, t->i - t->j);
		pack_add(pack, list, STR);
		t->j = t->i;
	}
}
