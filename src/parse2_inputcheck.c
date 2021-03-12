/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2_inputcheck.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:40:10 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/12 09:02:39 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

int	avoid_check(t_list *list, t_minishell *m_sh)
{
	char	*line;
	int		type;

	while (list)
	{
		packinfo_get(&line, &type, list);
		list = list->next;
		if (is_bonus(type))
			return (m_sh->exit_status = ft_avoid_error(line, 1));
	}
	return (0);
}

/*
**  ---syntax_check---
**  check dirs{<, >, >>} and metas{|, ;}
**  1. first meta OUT
**  2. meta meta OUT
**  3. dir meta OUT
**  4. dir dir OUT
**  5. dir EOF OUT
**
**  ---avoid check---
**  block newline
**  6. open quote BLOCK
**  7. | EOF BLOCK
**  8. ESC EOF BLOCK
**
**  9. BONUS metas{&&, ||, >>>} BLOCK
*/

int	input_check(t_list *store, t_minishell *m_sh)
{
	if (!store)
		return (0);
//	debug(store);
	return (syntax_check(store, m_sh) || avoid_check(store, m_sh));
}
