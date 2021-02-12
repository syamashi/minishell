/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_packmeta1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 19:01:30 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/13 01:50:15 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	token_pipe(t_pack **pack, t_list **list, t_token *t)
{
	pack_metaadd(pack, list, " ", SPACE);
	if (!(ft_strncmp(t->line + t->i, "||", 2)) && (++t->i))
		pack_metaadd(pack, list, "||", DPIPE);
	else
		pack_metaadd(pack, list, "|", PIPE);
	pack_metaadd(pack, list, " ", SPACE);
	t->j = t->i + 1;
}

void	token_and(t_pack **pack, t_list **list, t_token *t)
{
	pack_metaadd(pack, list, " ", SPACE);
	if (!(ft_strncmp(t->line + t->i, "&&", 2)) && (++t->i))
		pack_metaadd(pack, list, "&&", DAND);
	else
		pack_metaadd(pack, list, "&", AND);
	pack_metaadd(pack, list, " ", SPACE);
	t->j = t->i + 1;
}

void	token_redirect_l(t_pack **pack, t_list **list, t_token *t)
{
	pack_metaadd(pack, list, " ", SPACE);
	if (!(ft_strncmp(t->line + t->i, "<<<", 3)) && (t->i += 2))
		pack_metaadd(pack, list, "<<<", LLLDIR);
	else if (!(ft_strncmp(t->line + t->i, "<<", 2)) && ++t->i)
		pack_metaadd(pack, list, "<<", LLDIR);
	else
		pack_metaadd(pack, list, "<", LDIR);
	pack_metaadd(pack, list, " ", SPACE);
	t->j = t->i + 1;
}

void	token_redirect_r(t_pack **pack, t_list **list, t_token *t)
{
	pack_metaadd(pack, list, " ", SPACE);
	if (!(ft_strncmp(t->line + t->i, ">>", 2)) && ++t->i)
		pack_metaadd(pack, list, ">>", RRDIR);
	else
		pack_metaadd(pack, list, ">", RDIR);
	pack_metaadd(pack, list, " ", SPACE);
	t->j = t->i + 1;
}

void	token_semi(t_pack **pack, t_list **list, t_token *t)
{
	pack_metaadd(pack, list, " ", SPACE);
	pack_metaadd(pack, list, ";", SCOLON);
	t->j = t->i + 1;
}
