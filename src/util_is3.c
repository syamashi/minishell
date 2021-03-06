/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_is3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:36:36 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/16 21:55:43 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

bool	is_esc(char c)
{
	return (c == '"' ||
			c == '\\' ||
			c == '`' ||
			c == '$');
}

bool	is_envend(char c)
{
	return (is_space(c) ||
			c == '=' ||
			c == '$' ||
			c == '\0' ||
			c == '.' ||
			c == '~' ||
			c == '^' ||
			c == '+' ||
			c == ':' ||
			c == ',');
}

int		is_strsjoin(int type)
{
	return (type == STR ||
			type == SSTR ||
			type == ESC ||
			type == SQUOTE ||
			type == DQUOTE);
}
