/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsenew1_pack.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 16:17:58 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/08 02:25:31 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

void	add_space(t_pack **pack, t_list **list)
{
	pack_join(pack, " ", 1);
	pack_add(pack, list, SPACE);
}

void	token_meta(t_pack **pack, t_list **list, t_token *t, char c)
{
	if (t->j != t->i)
	{
		pack_join(pack, t->line + t->j, t->i - t->j);
		pack_add(pack, list, STR);
		t->j = t->i;
	}
	if (c == '|')
		token_pipe(pack, list, t);
	else if (c == '&')
		token_and(pack, list, t);
	else if (c == '<')
		token_redirect_l(pack, list, t);
	else if (c == '>')
		token_redirect_r(pack, list, t);
	else if (c == '\\')
		token_escape(pack, list, t);
	else if (c == '\'')
		token_squote(pack, list, t);
	else if (c == '"')
		token_dquote(pack, list, t);
	else if (c == ';')
		token_semi(pack, list, t);
}

t_list	*pack_end(t_pack **pack, t_list **list)
{
	if (*pack)
	{
		free((*pack)->line);
		(*pack)->line = NULL;
	}
	free(*pack);
	*pack = NULL;
	return (*list);
}

t_list	*ft_strtoken(char *line)
{
	t_token	t;
	t_list	*list;
	t_pack	*pack;

	def_strtoken(&t, &list, &line, &pack); //pack作る
	while (line[++t.i])
	{
		if (is_space(line[t.i]))
			add_space(&pack, &list);
		while (is_space(line[t.i]) && line[t.i])
			t.i++;
		t.j = t.i;
		--t.i;
		while (!is_space(line[++t.i]) && line[t.i])
			if (is_meta(line[t.i]))
				token_meta(&pack, &list, &t, line[t.i]);
		if (t.j != t.i)
			pack_join(&pack, line + t.j, t.i - t.j);
		if (*pack->line)
			pack_add(&pack, &list, STR);
		--t.i;
	}
	return (pack_end(&pack, &list));
}
