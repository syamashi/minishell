/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_pwdlist1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 10:50:51 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/14 12:31:45 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

void	lstlast_del(t_minishell *m_sh)
{
	t_list	*last;
	t_list	*pre;

	pre = NULL;
	last = m_sh->pwds;
	if (!last)
		return ;
	while (last->next)
	{
		pre = last;
		last = last->next;
	}
	if (pre)
	{
		pre->next = NULL;
		ft_lstdelone(last, free);
	}
	else
	{
		ft_lstdelone(m_sh->pwds, free);
		m_sh->pwds = NULL;
	}
}

void	pwdlst_cpy(t_minishell *sh, t_minishell *m_sh)
{
	t_list	*tmp;
	t_list	*new;

	tmp = m_sh->pwds;
	while (tmp)
	{
		new = ft_lstnew(ft_strdup((char *)(tmp->content)));
		ft_lstadd_back(&sh->pwds, new);
		tmp = tmp->next;
	}
}

void	pwdlst_update(t_minishell *m_sh, char *argv, int delflag)
{
	t_list	*work;
	t_list	*tmp;

	work = pwdlst_init(argv, delflag);
	if (delflag == NOCURRENT && (!*((char *)(ft_lstlast(m_sh->pwds))->content)))
		lstlast_del(m_sh);
	else if ((*argv == '/' || delflag == NXCURRENT) && !(m_sh->pwd_dslash = false))
		ft_lstclear(&m_sh->pwds, free);
	if (!ft_strncmp(argv, "//", 2) && ft_strncmp(argv, "///", 3))
		m_sh->pwd_dslash = true;
	tmp = work;
	while (tmp)
	{
		if (!ft_strncmp((char *)tmp->content, "..", 3))
			lstlast_del(m_sh);
		else if (delflag == NOCURRENT || ft_strncmp((char *)tmp->content, ".", 2))
			pwdlst_add(m_sh, (char *)tmp->content);
		tmp = tmp->next;
	}
	ft_lstclear(&work, free);
}

void	pwdlst_add(t_minishell *m_sh, char *str)
{
	char	*add;
	t_list	*new;

	if (!(add = ft_strdup(str)))
		exit(ft_error("malloc failed", 1));
	if (!(new = ft_lstnew(add)))
		exit(ft_error("malloc failed", 1));
	ft_lstadd_back(&m_sh->pwds, new);
}
