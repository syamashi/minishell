/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsenew1_packmeta2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 19:01:43 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/07 14:30:05 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	token_escape(t_pack **pack, t_list **list, t_token *t)
{
	if (t->line[++t->i])
		pack_join(pack, t->line + t->j + 1, 1);
	pack_add(pack, list, ESC);
	if (!t->line[t->i])
		t->i--;
	t->j = t->i + 1;
}

void	token_squote(t_pack **pack, t_list **list, t_token *t)
{
	pack_join(pack, "'", 1);
	pack_add(pack, list, SQUOTE);
	pack_join(pack, "", 0);
	pack_add(pack, list, STR);
	t->i++;
	t->j = t->i;
	while (t->line[t->i] != '\'' && t->line[t->i])
		t->i++;
	if (t->i != t->j)
	{
		pack_join(pack, t->line + t->j, t->i - t->j);
		pack_add(pack, list, SSTR);
	}
	if (t->line[t->i] == '\'')
	{
		pack_join(pack, "'", 1);
		pack_add(pack, list, SQUOTE);
	}
	if (!t->line[t->i])
		t->i--;
	t->j = t->i + 1;
}

void	token_dquote(t_pack **pack, t_list **list, t_token *t)
{
	pack_join(pack, "\"", 1);
	pack_add(pack, list, DQUOTE);
	pack_join(pack, "", 0);
	pack_add(pack, list, STR);
	t->j = t->i + 1;
	while (t->line[++t->i] && t->line[t->i] != '"')
		if (t->line[t->i] == '\\')
		{
			if (t->i != t->j)
			{
				pack_join(pack, t->line + t->j, t->i - t->j);
				pack_add(pack, list, STR);
				t->j = t->i;
			}
			token_escape(pack, list, t);
		}
	if (t->i != t->j)
	{
		pack_join(pack, t->line + t->j, t->i - t->j);
		pack_add(pack, list, STR);
	}
	if (t->line[t->i] == '"')
	{
		pack_join(pack, "\"", 1);
		pack_add(pack, list, DQUOTE);
	}
	if (!t->line[t->i])
		t->i--;
	t->j = t->i + 1;
}


/*void	token_doll(t_pack **pack, t_list **list, t_token *t)
{
	t->j = t->i;
	while (!is_space(t->line[t->i]) && !is_envmeta(t->line[t->i]) && t->line[t->i])
		t->i++;
	pack_join(pack, t->line + t->j, t->i - t->j);
	pack_add(pack, list, DOLL);
	t->j = t->i--;
}
*/