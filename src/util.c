/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 16:53:33 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/11 01:02:20 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_meta(char c)
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

int	is_envmeta(char c)
{
	return (c == '\'' ||
			c == '\"' ||
			c == '&' ||
			c == '<' ||
			c == '>' ||
			c == '|' ||
			c == ';');
}

int is_keyend(char c)
{
	return (is_space(c) ||
			c == '$' ||
			c == '\0');
}
