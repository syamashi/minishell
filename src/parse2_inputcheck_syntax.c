/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2_inputchecksyntax.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 13:15:31 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/12 14:15:05 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	check_quote(int type, int *quote_flag)
{
	if (type == SQUOTE)
		*quote_flag ^= 1;
	if (type == DQUOTE)
		*quote_flag ^= 2;
}

void	set_preinfo(int *type, int *pre_type, char **line, char **pre_line)
{
	*pre_type = (*type == SCOLON) ? -1 : *type;
	*pre_line = *line;
}

int	endline_check(int quote_flag, int pre_type, char *pre_line, t_minishell *m_sh)
{
	if (quote_flag)
		return (m_sh->exit_status = ft_avoid_error("open quote", 1));
	if (is_dir(pre_type))
		return (m_sh->exit_status = ft_syntax_error("newline", 258));
	if (pre_type == PIPE || (pre_type == ESC && !*pre_line))
		return (m_sh->exit_status = ft_avoid_error("multiline", 1));
	return (0);
}

int	syntax_check(t_list *list, t_minishell *m_sh)
{
	char	*line;
	char	*pre_line;
	int		type;
	int		pre_type;
	int		quote_flag;

	pre_type = -1;
	quote_flag = 0;
	while (list)
	{
		line = ((t_pack *)(list->content))->line;
		type = ((t_pack *)(list->content))->type;
		check_quote(type, &quote_flag);
		if ((pre_type == -1 || is_dir(pre_type) || is_metatype(pre_type)) && is_metatype(type))
			return (m_sh->exit_status = ft_syntax_error(line, 258));
		if (is_dir(pre_type) && is_dir(type))
			return (m_sh->exit_status = ft_syntax_error(line, 258));
		if (!isnot_cmd(type))
			set_preinfo(&type, &pre_type, &line, &pre_line);
		list = list->next;
	}
	return (endline_check(quote_flag, pre_type, pre_line, m_sh));
}
