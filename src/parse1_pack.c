/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_pack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 16:17:58 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/13 01:50:47 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	def_strtoken(t_token *t, t_list **list, char **line, t_pack **pack)
{
	t->line = *line;
	t->i = -1;
	*pack = NULL;
	new_pack(pack);
	*list = NULL;
}

void	token_meta(t_pack **pack, t_list **list, t_token *t, char c)
{
	pack_stradd(pack, list, t);
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

/*
**  {|, &, <, >, \, ', ", ;}
*/

t_list	*ft_strtoken(char *line)
{
	t_token	t;
	t_list	*list;
	t_pack	*pack;

	def_strtoken(&t, &list, &line, &pack);
	while (line[++t.i])
	{
		if (is_space(line[t.i]))
			pack_metaadd(&pack, &list, " ", SPACE);
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
