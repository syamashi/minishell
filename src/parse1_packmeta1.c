/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_packmeta1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 19:01:30 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/11 12:28:03 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	token_pipe(t_pack **pack, t_list **list, t_token *t)
{
	add_space(pack, list);
	if (!(ft_strncmp(t->line + t->i, "||", 2)) && (++t->i))
	{
		pack_join(pack, "||", 2);
		pack_add(pack, list, DPIPE);
	}
	else
	{
		pack_join(pack, "|", 1);
		pack_add(pack, list, PIPE);
	}
	add_space(pack, list);
	t->j = t->i + 1;
}

void	token_and(t_pack **pack, t_list **list, t_token *t)
{
	add_space(pack, list);
	if (!(ft_strncmp(t->line + t->i, "&&", 2)) && (++t->i))
	{
		pack_join(pack, "&&", 2);
		pack_add(pack, list, DAND);
	}
	else
	{
		pack_join(pack, "&", 1);
		pack_add(pack, list, AND);
	}
	add_space(pack, list);
	t->j = t->i + 1;
}

void	token_redirect_l(t_pack **pack, t_list **list, t_token *t)
{
	add_space(pack, list);
	if (!(ft_strncmp(t->line + t->i, "<<<", 3)) && (t->i += 2))
	{
		pack_join(pack, "<<<", 3);
		pack_add(pack, list, LLLDIR);
	}
	else if (!(ft_strncmp(t->line + t->i, "<<", 2)) && ++t->i)
	{
		pack_join(pack, "<<", 2);
		pack_add(pack, list, LLDIR);
	}
	else
	{
		pack_join(pack, "<", 1);
		pack_add(pack, list, LDIR);
	}
	add_space(pack, list);
	t->j = t->i + 1;
}

void	token_redirect_r(t_pack **pack, t_list **list, t_token *t)
{
	add_space(pack, list);
	if (!(ft_strncmp(t->line + t->i, ">>", 2)) && ++t->i)
	{
		pack_join(pack, ">>", 2);
		pack_add(pack, list, RRDIR);
	}
	else
	{
		pack_join(pack, ">", 1);
		pack_add(pack, list, RDIR);
	}
	add_space(pack, list);
	t->j = t->i + 1;
}

void	token_semi(t_pack **pack, t_list **list, t_token *t)
{
	add_space(pack, list);
	pack_join(pack, ";", 1);
	pack_add(pack, list, SCOLON);
	add_space(pack, list);
	t->j = t->i + 1;
}
