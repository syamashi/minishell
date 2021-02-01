/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_packmeta1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 19:01:30 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/31 21:06:38 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	token_pipe(t_pack **pack, t_list **list, t_token *t, int type)
{
	if (t->j != t->i)
		if (pack_join(pack, t->line + t->j, t->i - t->j))
			return (1);
	if (*(*pack)->line)
		if (pack_add(pack, list, 0))
			return (1);
	if (!(ft_strncmp(t->line + t->i, "||", 2)) && (++t->i))
	{
		if (pack_join(pack, "||", 2))
			return (1);
		if (pack_add(pack, list, 4))
			return (1);
	}
	else
	{
		if (pack_join(pack, "|", 1))
			return (1);
		if (pack_add(pack, list, type))
			return (1);
	}
	t->j = t->i + 1;
	return (0);
}

int	token_and(t_pack **pack, t_list **list, t_token *t, int type)
{
	if (t->j != t->i)
		if (pack_join(pack, t->line + t->j, t->i - t->j))
			return (1);
	if (*(*pack)->line)
		if (pack_add(pack, list, 0))
			return (1);
	if (!(ft_strncmp(t->line + t->i, "&&", 2)) && (++t->i))
	{
		if (pack_join(pack, "&&", 2))
			return (1);
		if (pack_add(pack, list, 5))
			return (1);
	}
	else
	{
		if (pack_join(pack, "&", 1))
			return (1);
		if (pack_add(pack, list, type))
			return (1);
	}
	t->j = t->i + 1;
	return (0);
}

int	token_redirect_l(t_pack **pack, t_list **list, t_token *t, int type)
{
	if (t->j != t->i)
		if (pack_join(pack, t->line + t->j, t->i - t->j))
			return (1);
	if (*(*pack)->line)
		if (pack_add(pack, list, 0))
			return (1);
	if (!(ft_strncmp(t->line + t->i, "<<<", 3)) && (t->i += 2))
		if (pack_join(pack, "<<<", 3))
			return (1);
	else if (!(ft_strncmp(t->line + t->i, "<<", 2)) && ++t->i)
		if (pack_join(pack, "<<", 2))
			return (1);
	else
		if (pack_join(pack, "<", 1))
			return (1);
	if (pack_add(pack, list, type))
		return (1);
	t->j = t->i + 1;
	return (0);
}

int	token_redirect_r(t_pack **pack, t_list **list, t_token *t, int type)
{
	if (t->j != t->i)
		if (pack_join(pack, t->line + t->j, t->i - t->j))
			return (1);
	if (*(*pack)->line)
		if (pack_add(pack, list, 0))
			return (1);
	if (!(ft_strncmp(t->line + t->i, ">>", 2)) && ++t->i)
		if (pack_join(pack, ">>", 2))
			return (1);
	else
		if (pack_join(pack, ">", 1))
			return (1);
	if (pack_add(pack, list, type))
		return (1);
	t->j = t->i + 1;
	return (0);
}

int	token_escape(t_pack **pack, t_list **list, t_token *t)
{
	if (t->j != t->i)
		if (pack_join(pack, t->line + t->j, t->i - t->j))
			return (1);
	if (pack_join(pack, t->line + t->i + 1, 1))
		return (1);
	++t->i;
	t->j = t->i + 1;
	return (0);
}
