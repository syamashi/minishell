/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsenew2_syntaxcheck.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:40:10 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/08 02:12:17 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_dir(int n)
{
	return (n == RDIR ||
			n == RRDIR ||
			n == LDIR ||
			n == LLDIR ||
			n == LLLDIR);
}

int	is_metatype(int n)
{
	return (n == PIPE ||
			n == DPIPE ||
			n == AND ||
			n == DAND ||
			n == SCOLON);
}

int is_bonus(int n)
{
	return (n == LLDIR ||
			n == LLLDIR ||
			n == DPIPE ||
			n == AND ||
			n == DAND);
}


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

int	is_skip(const int type)
{
	return (type == SPACE ||
			type == SQUOTE ||
			type == DQUOTE);
}

/*
**  check dirs{<, >, >>} and metas{|, ;}
**  1. first meta OUT
**  2. meta meta OUT
**  3. dir meta OUT
**  4. dir dir OUT
**  5. dir EOF OUT
**
**  block newline
**  6. open quote BLOCK
**  7. | EOF BLOCK
**  8. ESC EOF BLOCK
*/

static int	syntax_check(t_list *list)
{
	char	*line;
	int		type;
	char	*pre_line;
	int		pre_type;
	int		quote_flag;

	pre_line = ((t_pack *)(list->content))->line;
	pre_type = -1;
	quote_flag = 0;
	while (list)
	{
		line = ((t_pack *)(list->content))->line;
		type = ((t_pack *)(list->content))->type;
		check_quote(type, &quote_flag);
		if ((pre_type == -1 || is_dir(pre_type) || is_metatype(pre_type)) && is_metatype(type))
			return (ft_syntax_error(line, 258));
		if (is_dir(pre_type) && is_dir(type))
			return (ft_syntax_error(line, 258));
		if (!is_skip(type))
			set_preinfo(&type, &pre_type, &line, &pre_line);
		list = list->next;
	}
	if (quote_flag)
		return (ft_avoid_error("open quote", 1));
	if (is_dir(pre_type))
		return (ft_syntax_error("newline", 258));
	if (pre_type == PIPE || (pre_type == ESC && !*pre_line))
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
		if (is_bonus(type))
			return (ft_avoid_error(line, 1));
	}
	return (0);
}

int	input_check(t_list *store)
{
	int r;

	if (!store)
		return (0);
	if (r = syntax_check(store))
		return (r);
	if (r = avoid_check(store))
		return (r);
	return (r);
}
