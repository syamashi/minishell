/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_checksyntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:40:10 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/30 19:48:24 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	syntax_check(t_list *list)
{
	int		pre_type;
	char	*pre_line;

	pre_type = -1;
	pre_line = NULL;
	while (list)
	{
		if (pre_type == -1 && ((t_pack*)(list->content))->type == 1)
			return(ft_syntax_error(((t_pack*)(list->content))->line, 1));
		if (pre_type > 0 && ((t_pack*)(list->content))->type)
			return (ft_syntax_error(((t_pack*)(list->content))->line, 1));
		pre_type = ((t_pack*)(list->content))->type;
		pre_line = ((t_pack*)(list->content))->line;
		list = list->next;
	}
	if (pre_type == 2)
		return (ft_syntax_error("newline", 1));
	if (pre_type == 1)
		return (ft_avoid_error("multiline", 1));
	return (0);
}

static int	avoid_check(t_list *list)
{
	char	*line;
	int		type;
	
	while (list)
	{
		line = ((t_pack *)(list->content))->line;
		type = ((t_pack *)(list->content))->type;
		list = list->next;
		if (type)
		{
			if (!ft_strncmp(line, "<", 2))
				continue;
			if (!ft_strncmp(line, ">", 2))
				continue;
			if (!ft_strncmp(line, ">>", 3))
				continue;
			if (!ft_strncmp(line, "|", 2))
				continue;
			return (ft_avoid_error(line, 1));			
		}
	}
	return (0);
}

int	meta_check(t_list **packs)
{
	t_list *semi;

	semi = *packs;
	while (semi)
	{
		if (syntax_check(semi->content) || avoid_check(semi->content))
		{
			ft_lstclear(packs, pack_free);
			return (1);
		}
		semi = semi->next;
	}
	return (0);
}
