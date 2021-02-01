/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 18:16:12 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/30 18:11:05 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void pack_free(void *ptr)
{
//	printf("[packfree] %s\n", ((t_pack *)(ptr))->line);
	free(((t_pack *)(ptr))->line);
	((t_pack *)ptr)->line = NULL;
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

void all_free(t_list **store, t_list **env)
{
	t_list *tmp;

//	get_next_line(-1, NULL);
//	printf("[all_free]\n");
	if (store)
	{
		while (*store)
		{
			ft_lstclear((t_list**)&(*store)->content, exlist_free);
			tmp = (*store)->next;
			ft_lstdelone(*store, free);
			*store = tmp;
		}
		*store = NULL;
	}
	if (env)
		ft_lstclear(env, env_free);
//	printf("[all_free]end\n");
}
