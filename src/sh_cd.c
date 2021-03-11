/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:23:23 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/11 23:10:14 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

/*
** export_envp() required malloced key/value and freed
*/

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
		pre->next = NULL;
	ft_lstdelone(last, free);
}

void	pwdlst_update(t_minishell *m_sh, char *argv, int delflag)
{
	t_list	*work;
	t_list	*tmp;
	t_list	*new;
	char	*dir;

	work = pwdlst_init(argv, delflag);
	if (delflag == NOCURRENT && (!*((char *)(ft_lstlast(m_sh->pwds))->content)))
		lstlast_del(m_sh);
	else if (*argv == '/' || delflag == NXCURRENT)
		ft_lstclear(&m_sh->pwds, free);
	tmp = work;
	while (tmp)
	{
		if (!ft_strncmp((char *)tmp->content, "..", 3))
			lstlast_del(m_sh);
		else if (delflag == NOCURRENT || ft_strncmp((char *)tmp->content, ".", 2))
		{
			if (!(dir = ft_strdup((char *)tmp->content)) || !(new = ft_lstnew(dir)))
				exit(ft_error("", 1));
			ft_lstadd_back(&m_sh->pwds, new);
		}
		tmp = tmp->next;
	}
	ft_lstclear(&work, free);
}

char	*pwds_str(t_minishell *m_sh)
{
	char	*pwd;
	char	*tmp;
	t_list	*list;

	pwd = NULL;
	list = m_sh->pwds;
	while (list)
	{
		tmp = pwd;
		if (!(pwd = ft_strjoin(pwd, "/")))
			exit(ft_error("", 1));
		free(tmp);
		tmp = pwd;
		char *dir = (char*)list->content;
		if (!(pwd = ft_strjoin(pwd, dir)))
			exit(ft_error("", 1));
		free(tmp);
		list = list->next;
	}
	return (pwd);
}

int		pwd_update(t_minishell *m_sh, char *argv, int delflag)
{
	char	*pwdval;

	pwdval = value_get("PWD", m_sh);
	if (key_find("OLDPWD", m_sh))
		export_envp(m_sh, ft_strdup("OLDPWD"), pwdval);
	else
		free(pwdval);
	pwdlst_update(m_sh, argv, delflag);
	if (delflag == NXCURRENT)
		free(argv);
	pwdval = pwds_str(m_sh);
	errno = 0;
	if (key_find("PWD", m_sh))
		export_envp(m_sh, ft_strdup("PWD"), pwdval);
	else
		free(pwdval);
	return (0);
}

int		cd_no_args(t_minishell *m_sh, char *argv)
{
	char	*home;

	if (!(home = value_get("HOME", m_sh)))
		return (ft_cd_error(NULL, 1, "HOME not set"));
	if (chdir(home) == -1)
	{
		free(home);
		return (ft_cd_error(argv, 1, "No such file or directory"));
	}
	ft_lstclear(&m_sh->pwds, free);
	pwd_update(m_sh, home, false);
	free(home);
	return (0);
}

int		cd_no_current(t_minishell *m_sh, char *argv)
{
	pwd_update(m_sh, argv, NOCURRENT);
	ft_putstr_fd("cd: error retrieving current directory: ", 2);
	ft_putstr_fd("getcwd: cannot access parent directories : ", 2);
	ft_putstr_fd("No such file or directory\n", 2);
	return (1);
}

int		sh_cd(t_minishell *m_sh, t_exec *exec)
{
	char			**argv;
	char			*pwd;
	int				chdir_ret;
	char			*path;
	static	bool	nocurrent;

	errno = 0;
	argv = exec->argv + 1;
	if (!*argv)
		return (cd_no_args(m_sh, *argv));
	if (ft_strlen(*argv) > 255)
		return (ft_cd_error(*argv, 1, "File name too long"));
	if (!**argv)
		return (pwd_update(m_sh, *argv, false));
	if (chdir(*argv))
		return (ft_cd_error(*argv, 1, "No such file or directory"));
	if (!(path = getcwd(NULL, 0)) && (nocurrent = true))
		return (cd_no_current(m_sh, *argv));
	if (nocurrent && !(nocurrent = false))
		return (pwd_update(m_sh, path, NXCURRENT));
	free(path);
	return (pwd_update(m_sh, *argv, 0));
}
