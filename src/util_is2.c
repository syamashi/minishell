/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_is2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:35:56 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/20 12:58:04 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

bool	is_dir(int n)
{
	return (n == RDIR ||
			n == RRDIR ||
			n == LDIR ||
			n == LLDIR ||
			n == LLLDIR);
}

bool	is_metatype(int n)
{
	return (n == PIPE ||
			n == DPIPE ||
			n == AND ||
			n == DAND ||
			n == SCOLON ||
			n == DSCOLON);
}

bool	is_bonus(int n)
{
	return (n == LLDIR ||
			n == LLLDIR ||
			n == AND);
}

bool	isnot_cmd(const int type)
{
	return (type == SPACE ||
			type == SQUOTE ||
			type == DQUOTE);
}

bool	is_cmd(int type)
{
	return (type == STR ||
			is_dir(type));
}
