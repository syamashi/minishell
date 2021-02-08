/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 18:16:12 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/08 01:30:10 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void pack_free(void *ptr)
{
//	printf("[packfree] %s\n", ((t_pack*)ptr)->line);
	free(((t_pack*)ptr)->line);
	((t_pack*)ptr)->line = NULL;
	free(ptr);
	ptr = NULL;
}

void packs_free(t_list **packs)
{
	t_list *mov;

	while (*packs)
	{
		mov = (*packs)->next;
		ft_lstclear((t_list**)&((*packs)->content), pack_free);
		
		*packs = mov;
	}
}

void env_free(void *ptr)
{
	free(((t_dict *)ptr)->key);
	((t_dict *)ptr)->key = NULL;
	free(((t_dict *)ptr)->value);
	((t_dict *)ptr)->value = NULL;
	free(ptr);
	ptr = NULL;
}

void exlist_free(void *ptr)
{
	int	i;

	i = 0;
	while (((t_exec *)ptr)->argv[i])
	{
//		printf("[exlist_free] argv[%d]:%s\n", i, (((t_exec *)ptr)->argv[i]));
		free(((t_exec *)ptr)->argv[i]);
		((t_exec *)ptr)->argv[i++] = NULL;
	}
	free(((t_exec *)ptr)->argv);
	free(ptr);
	ptr = NULL;
}

void	store_free(t_list **store)
{
	t_list	*packs;
	t_list	*tmp;
	
	while (*store)
	{
		packs = (*store)->content;
		tmp = (*store)->next;
		ft_lstclear(&packs, pack_free);
		free(*store);
		*store = tmp;
	}
}

void	ast_free(t_list **ast)
{
	t_list	*packs;
	t_list	*tmp;
	
	while (*ast)
	{
		tmp = (*ast)->next;
		packs = ((t_leaf*)(*ast)->content)->dir;
		ft_lstclear(&packs, pack_free);
		packs = ((t_leaf*)(*ast)->content)->str;
		ft_lstclear(&packs, pack_free);
		ft_lstdelone(*ast, free);
		*ast = NULL;
		*ast = tmp;
	}
}

void all_free(char **line, t_list **store, t_list **ast)
{
	if (line)
	{
		free(*line);
		*line = NULL;
	}
	if (store)
		store_free(store);
	if (ast)
		ast_free(ast);
//	if (env)
//		ft_lstclear(env, env_free);
}
