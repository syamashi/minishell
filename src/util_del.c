/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_del.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 12:45:56 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/14 12:47:20 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_vlstclear(void *vlst_arg, void (*del)(void*))
{
	void	**vlst;
	void	**it;

	vlst = vlst_arg;
	it = vlst;
	while (*it)
		del(*it++);
	free(vlst);
}

void	del_t_exec(void *exec_arg)
{
	t_exec	*exec;

	exec = exec_arg;
	ft_vlstclear(exec->argv, free);
	ft_vlstclear(exec->envp, free);
	free(exec);
}

void	del_pack(void *pack_arg)
{
	t_pack *pack;

	pack = pack_arg;
	free(pack->line);
	free(pack);
}

void	del_command(void *pack_list_arg)
{
	t_list	*pack_list;

	pack_list = pack_list_arg;
	ft_lstclear(&pack_list, del_pack);
}
