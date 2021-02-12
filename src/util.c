/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 16:53:33 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/11 21:15:09 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

bool	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

bool	is_quote(int n)
{
	return (n == SQUOTE ||
			n == DQUOTE);
}

bool	is_meta(char c)
{
	return (c == '\'' ||
			c == '\"' ||
			c == '&' ||
			c == '<' ||
			c == '>' ||
			c == '|' ||
			c == '\\' ||
			c == ';');
}

bool	is_envmeta(char c)
{
	return (c == '\'' ||
			c == '\"' ||
			c == '&' ||
			c == '<' ||
			c == '>' ||
			c == '|' ||
			c == ';');
}

bool	is_keyend(char c)
{
	return (is_space(c) ||
			c == '$' ||
			c == '\0');
}

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
			n == SCOLON);
}

bool	is_bonus(int n)
{
	return (n == LLDIR ||
			n == LLLDIR ||
			n == DPIPE ||
			n == AND ||
			n == DAND);
}

