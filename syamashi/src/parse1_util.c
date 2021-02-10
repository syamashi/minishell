/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:50:00 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/10 16:05:53 by syamashi         ###   ########.fr       */
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

void	new_pack(t_pack **pack)
{
	if (!(*pack = (t_pack *)malloc(sizeof(t_pack))))
		exit(ft_error("minishell: malloc failed", 1));
	if (!((*pack)->line = ft_strdup("")))
		exit(ft_error("minishell: malloc failed", 1));
	(*pack)->type = STR;
}

void	pack_join(t_pack **pack, char *str, int len)
{
	char *tmp;
	char *add;

	tmp = (*pack)->line;
	if (!(add = ft_substr(str, 0, len)))
		exit(ft_error("minishell: malloc failed", 1));
	if (!((*pack)->line = ft_strjoin((*pack)->line, add)))
		exit(ft_error("minishell: malloc failed", 1));
	free(tmp);
	tmp = NULL;
	free(add);
	add = NULL;
}

void	pack_add(t_pack **pack, t_list **list, int type)
{
	t_list *new;

	(*pack)->type = type;
	if (!(new = ft_lstnew(*pack)))
		exit(ft_error("minishell: malloc failed", 1));
	ft_lstadd_back(list, new);
	new_pack(pack);
}
