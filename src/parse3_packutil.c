/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_packutil.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:50:00 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/31 18:05:25 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	def_strtoken(t_token *t, t_list **list, char **line, t_pack **pack)
{
	t->line = *line;
	t->i = -1;
	*pack = NULL;
	*list = NULL;
}

int	new_pack(t_pack **pack)
{
	if (!(*pack = (t_pack *)malloc(sizeof(t_pack))))
		return (ft_error("[new_pack] pack malloc error", 1));
	if (!((*pack)->line = ft_strdup("")))
	{
		free(*pack);
		*pack = NULL;
		return (ft_error("[new_pack] (*pack)->line malloc error", 1));
	}
	(*pack)->type = 0;
	return (0);
}

int	pack_join(t_pack **pack, char *str, int len)
{
	char *tmp;
	char *add;

	tmp = (*pack)->line;
	if (!(add = ft_substr(str, 0, len)))
		return (ft_error("[pack_join] add malloc failed", 1));
	if (!((*pack)->line = ft_strjoin((*pack)->line, add)))
		return (ft_error("[pack_join] (*pack)->line malloc failed", 1));
	free(tmp);
	tmp = NULL;
	free(add);
	add = NULL;
	return (0);
}

int pack_add(t_pack **pack, t_list **list, int type)
{
	t_list *new;

	(*pack)->type = type;
	if (!(new = ft_lstnew(*pack)))
		return (ft_error("[pack_add] new malloc failed", 1));
	ft_lstadd_back(list, new);
	if (new_pack(pack))
		return (1);
	return (0);
}
