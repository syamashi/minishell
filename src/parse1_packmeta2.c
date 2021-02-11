/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_packmeta2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 19:01:43 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/11 14:46:16 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

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
	pack_metaadd(pack, list, "'", SQUOTE);
	pack_metaadd(pack, list, "", STR);
	t->i++;
	t->j = t->i;
	while (t->line[t->i] != '\'' && t->line[t->i])
		t->i++;
	pack_stradd(pack, list, t);
	if (t->line[t->i] == '\'')
		pack_metaadd(pack, list, "'", SQUOTE);
	if (!t->line[t->i])
		t->i--;
	t->j = t->i + 1;
}

/*
**  ESC in DQUOTE work 4chars { \"$` } only
*/

bool	is_dqoteesc(const char *line, const int i)
{
	return (!ft_strncmp(line + i, "\\\\", 2) ||
			!ft_strncmp(line + i, "\\\"", 2) ||
			!ft_strncmp(line + i, "\\$", 2) ||
			!ft_strncmp(line + i, "\\`", 2));
}

void	token_dquote(t_pack **pack, t_list **list, t_token *t)
{
	pack_metaadd(pack, list, "\"", DQUOTE);
	pack_metaadd(pack, list, "", STR);
	t->j = t->i + 1;
	while (t->line[++t->i] && t->line[t->i] != '"')
		if (is_dqoteesc(t->line, t->i))
		{
			pack_stradd(pack, list, t);
			token_escape(pack, list, t);
		}
	pack_stradd(pack, list, t);
	if (t->line[t->i] == '"')
		pack_metaadd(pack, list, "\"", DQUOTE);
	if (!t->line[t->i])
		t->i--;
	t->j = t->i + 1;
}
