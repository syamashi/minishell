/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 18:16:12 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/17 20:01:47 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pack_free(void *ptr)
{
	free(((t_pack*)ptr)->line);
	((t_pack*)ptr)->line = NULL;
	free(ptr);
	ptr = NULL;
}

void	env_free(void *ptr)
{
	free(((t_dict *)ptr)->key);
	((t_dict *)ptr)->key = NULL;
	free(((t_dict *)ptr)->value);
	((t_dict *)ptr)->value = NULL;
	free(ptr);
	ptr = NULL;
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
