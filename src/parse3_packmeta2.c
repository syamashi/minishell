/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_packmeta2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 19:01:43 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/31 20:59:44 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	token_squote(t_pack **pack, t_list **list, t_token *t)
{
	if (t->j != t->i)
		if (pack_join(pack, t->line + t->j, t->i - t->j))
			return (1);
	t->j = t->i + 1;
	while (t->line[++t->i] != '\'')
		;
	if (pack_join(pack, t->line + t->j, t->i - t->j))
		return (1);
	t->j = t->i + 1;
	return (0);
}

int	token_dquote(t_pack **pack, t_list **list, t_token *t)
{
	if (t->j != t->i)
		if (pack_join(pack, t->line + t->j, t->i - t->j))
			return (1);
	t->j = t->i + 1;
	while (t->line[++t->i] != '"')
		;
	if (pack_join(pack, t->line + t->j, t->i - t->j))
		return (1);
	t->j = t->i + 1;
	return (0);
}
