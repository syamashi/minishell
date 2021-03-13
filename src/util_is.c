/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_is.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:34:43 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/13 12:36:10 by syamashi         ###   ########.fr       */
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
			c == '=' ||
			c == '$' ||
			c == '\0')
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
