/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_pack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:40:17 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/31 21:07:16 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pack_error(t_list **newcnt, t_list **lines, t_list **packs)
{
	if (newcnt)
	{
		ft_lstclear(newcnt, pack_free);
		ft_error("[packed] newlst malloc failed", 1);
	}
	ft_lstclear(lines, free);
	ft_lstclear(packs, pack_free);
	return (1);
}

int	token_meta(t_pack **pack, t_list **list, t_token *t, char c)
{
	int r;

	r = 0;
	if (c == '|')
		r = token_pipe(pack, list, t, 1); // || = 4
	else if (c == '&')
		r = token_and(pack, list, t, 3); // && = 5
	else if (c == '<')
		r = token_redirect_l(pack, list, t, 2);
	else if (c == '>')
		r = token_redirect_r(pack, list, t, 2);
	else if (c == '\\')
		r = token_escape(pack, list, t);
	else if (c == '\'')
		r = token_squote(pack, list, t);
	else if (c == '"')
		r = token_dquote(pack, list, t);
	return (r);
}

t_list	*pack_end(t_pack **pack, t_list **list, int error_flag)
{
	if (*pack)
		free((*pack)->line);
	free(*pack);
	if (error_flag)
		return (NULL);
	return (*list);
}

static t_list	*ft_strtoken(char *line)
{
	t_token	t;
	t_list	*list;
	t_pack	*pack;

	def_strtoken(&t, &list, &line, &pack);
	while (line[++t.i])
	{
		while (is_space(line[t.i]))
			t.i++;
		if (!pack)
			if (new_pack(&pack))
				return (NULL);
		t.j = t.i;
		--t.i;
		while (!is_space(line[++t.i]) && line[t.i])
			if (is_meta(line[t.i]))
				if (token_meta(&pack, &list, &t, line[t.i]))
					return (pack_end(&pack, &list, 1));
		if (t.j != t.i)
			if (pack_join(&pack, line + t.j, t.i - t.j))
				return (pack_end(&pack, &list, 1));
		if (*pack->line)
			if (pack_add(&pack, &list, 0))
				return (pack_end(&pack, &list, 1));
		--t.i;
	}
	return (pack_end(&pack, &list, 0));
}

int	packed(t_list **packs, t_list **lines)
{
	t_list	*mov;
	t_list	*newcnt;
	t_list	*newlst;

	mov = *lines;
	newcnt = NULL;
	newlst = NULL;
	while (mov)
	{
		if (!(newcnt = ft_strtoken((char*)mov->content)))
			return (pack_error(NULL, lines, packs));
		if (!(newlst = ft_lstnew(newcnt)))
			return (pack_error(&newcnt, lines, packs));
		ft_lstadd_back(packs, newlst);
		mov = mov->next;
	}
	return (0);
}
