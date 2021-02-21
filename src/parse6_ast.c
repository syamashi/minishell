/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse6_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 01:33:38 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/21 03:16:35 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	ast_add(t_list **ast, t_leaf **leaf)
{
	t_list	*new;

	if (!(new = ft_lstnew(*leaf)))
		exit(ft_error("minishell: malloc failed", 1));
	ft_lstadd_back(ast, new);
}

void	new_leaf(t_leaf **leaf)
{
	if (!(*leaf = (t_leaf *)malloc(sizeof(t_leaf))))
		exit(ft_error("minishell: malloc failed", 1));
	(*leaf)->str = NULL;
	(*leaf)->dir = NULL;
}

void	dir_add(t_list **mov, t_leaf **leaf)
{
	t_pack	*pack;
	t_list	*new;
	char	*line;

	while (1)
	{
		new_pack(&pack);
		line = ((t_pack*)((*mov)->content))->line;
		pack_join(&pack, line, ft_strlen(line));
		pack->type = ((t_pack*)((*mov)->content))->type;
		if (!(new = ft_lstnew(pack)))
			exit(ft_error("minishell: malloc failed", 1));
		ft_lstadd_back(&(*leaf)->dir, new);
		if (((t_pack*)((*mov)->content))->type == STR)
			break;
		*mov = (*mov)->next;
	}
}

void	str_add(t_list **mov, t_leaf **leaf)
{
	t_pack	*pack;
	t_list	*new;
	char	*line;

	new_pack(&pack);
	line = ((t_pack*)((*mov)->content))->line;
	pack_join(&pack, line, ft_strlen(line));
	pack->type = ((t_pack*)((*mov)->content))->type;
	if (!(new = ft_lstnew(pack)))
		exit(ft_error("minishell: malloc failed", 1));
	ft_lstadd_back(&(*leaf)->str, new);
}

/*
**  ast[0] -(pipe)- ast[1] -(pipe)- ast[2]
**  -leaf->str
**  -leaf->dir
*/

void	ast_init(t_list **ast, t_list **packs)
{
	t_list	*new;
	t_list	*pipe;
	t_leaf	*leaf;
	t_list	*mov;
	int		type;

	mov = *packs;
	new_leaf(&leaf);
	while (mov)
	{
		type = ((t_pack *)mov->content)->type;
		if (type == PIPE)
		{
			ast_add(ast, &leaf);
			new_leaf(&leaf);
		}
		else if (is_dir(type) || type == RINT)
			dir_add(&mov, &leaf);
		else
			str_add(&mov, &leaf);
		mov = mov->next;
	}
	ast_add(ast, &leaf);
}
