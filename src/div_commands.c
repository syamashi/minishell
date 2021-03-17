/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   div_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 12:40:04 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/17 19:01:23 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

bool		is_nums(char *line)
{
	int i;

	i = -1;
	while (ft_isdigit(line[++i]))
		;
	return (line[i] ? false : true);
}

void		redirectionint_trance(t_list *store)
{
	t_list	*mov;
	int		type;
	char	*line;
	int		ptype;

	mov = store;
	ptype = -1;
	while (mov && mov->next)
	{
		line = ((t_pack *)mov->content)->line;
		type = ((t_pack *)mov->content)->type;
		if (is_dir(((t_pack*)mov->next->content)->type) && type == STR
		&& (ptype == -1 || ptype == SPACE))
			if (is_nums(line))
				((t_pack *)mov->content)->type = RINT;
		ptype = type;
		mov = mov->next;
	}
}

t_command	*div_commands(t_minishell *m_sh, char *line)
{
	t_command	*store;

	store = ft_clstnew(ft_strtoken(line));
	redirectionint_trance(store->content);
	if (input_check(store->content, m_sh))
	{
		ft_clstdelone(store, del_command);
		return (NULL);
	}
	store_div(&store);
	return (store);
}
