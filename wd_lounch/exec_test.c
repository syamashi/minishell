/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 15:48:07 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/06 14:31:41 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_lounch.h"

t_exec	*simple_cat()
{
	static char *argv[] = {"cat", "input.txt", NULL};
	static t_exec	ex_simple_cat = {NULL, NULL, 0, 1};
	ex_simple_cat.argv = argv;
	ex_simple_cat.envp = g_envp;
	return (&ex_simple_cat);
}

t_exec	*only_cat()
{
	static char *argv[] = {"cat", NULL};
	static t_exec	ex_simple_cat = {NULL, NULL, 0, 1};
	ex_simple_cat.argv = argv;
	ex_simple_cat.envp = g_envp;
	return (&ex_simple_cat);
}

t_exec	*only_ls()
{
	static char *argv[] = {"ls", NULL};
	static t_exec	ex_simple_cat = {NULL, NULL, 0, 1};
	ex_simple_cat.argv = argv;
	ex_simple_cat.envp = g_envp;
	return (&ex_simple_cat);
}

t_exec	*grep_c()
{
	static char *argv[] = {"grep", "pipe", NULL};
	static t_exec	ex_simple_cat = {NULL, NULL, 0, 1};
	ex_simple_cat.argv = argv;
	ex_simple_cat.envp = g_envp;
	return (&ex_simple_cat);
}

t_list	*simple_cat_lst()
{
	static t_list	*lst;
	lst = ft_lstnew(simple_cat());
	return (lst);
}

t_list	*pipe_cat_lst()
{
	static t_list	*lst;
	lst = ft_lstnew(simple_cat());
	ft_lstadd_back(&lst, ft_lstnew(only_cat()));
	return (lst);
}

t_list	*ls_and_grep()
{
	static t_list	*lst;
	lst = ft_lstnew(only_ls());
	ft_lstadd_back(&lst, ft_lstnew(grep_c()));
	return (lst);
}

int		main(int argc, char **argv, char **envp)
{
	g_envp = envp;
	t_list *ex_lst = ls_and_grep();
	sh_lounch(ex_lst);
	return (0);
}
