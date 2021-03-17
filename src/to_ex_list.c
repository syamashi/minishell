/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_ex_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 12:36:03 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/16 20:35:41 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

bool	is_tilde(char *line)
{
	if (ft_strncmp(line, "~/", 2) && ft_strncmp(line, "~", 2))
		return (false);
	return (true);
}

void	tilde_join(t_list *mov, t_minishell *m_sh)
{
	char	*line;
	char	*tmp;
	char	*home_value;

	line = ((t_pack*)mov->content)->line;
	if (!(home_value = value_get("HOME", m_sh)))
		if (!(home_value = ft_strdup(m_sh->home_defvalue)))
			if (!(home_value = ft_strdup("")))
				exit(ft_error("malloc failed", 1, STDERR));
	tmp = line;
	line = ft_strjoin(home_value, line + 1);
	free(tmp);
	tmp = NULL;
	free(home_value);
	home_value = NULL;
	((t_pack *)mov->content)->line = line;
}

void	tilde_expand(t_list **pack_list, t_minishell *m_sh)
{
	t_list	*mov;
	char	*line;
	int		type;
	int		pre_type;

	mov = *pack_list;
	pre_type = SPACE;
	while (mov)
	{
		packinfo_get(&line, &type, mov);
		if (pre_type == SPACE && type == STR && is_tilde(line))
			tilde_join(mov, m_sh);
		pre_type = type;
		mov = mov->next;
	}
}

t_list	*to_ex_list(t_minishell *m_sh, t_list **pack_list)
{
	t_list	*ex_list;
	t_list	*ast;

	ast = NULL;
	ex_list = NULL;
	tilde_expand(pack_list, m_sh);
	env_expand((t_list **)pack_list, m_sh, 0);
	packs_trim((t_list **)pack_list);
	ast_init(&ast, (t_list**)pack_list);
	exlist_init(ast, &ex_list, m_sh);
	ast_free(&ast);
	return (ex_list);
}
