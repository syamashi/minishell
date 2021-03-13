/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:23:23 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/13 12:26:02 by syamashi         ###   ########.fr       */
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

void	pwdlst_update(t_minishell *m_sh, char *argv, int delflag)
{
	t_list	*work;
	t_list	*tmp;
	t_list	*new;
	char	*dir;

//	printf("[pwdlst_update]argv:%s\n", argv);
	work = pwdlst_init(argv, delflag);
	if (delflag == NOCURRENT && (!*((char *)(ft_lstlast(m_sh->pwds))->content)))
		lstlast_del(m_sh);
	else if ((*argv == '/' || delflag == NXCURRENT) && !(m_sh->pwd_dslash = false))
		ft_lstclear(&m_sh->pwds, free);
	if (!ft_strncmp(argv, "//", 2) && ft_strncmp(argv, "///", 3))
		m_sh->pwd_dslash = true;
//	printf("[pwdlst_update]dslash:%d\n", m_sh->pwd_dslash);
	tmp = work;
	while (tmp)
	{
		if (!ft_strncmp((char *)tmp->content, "..", 3))
			lstlast_del(m_sh);
		else if (delflag == NOCURRENT || ft_strncmp((char *)tmp->content, ".", 2))
			pwdlst_add(m_sh, (char *)tmp->content);
//		printf("[pwdlst_update]content:%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
	ft_lstclear(&work, free);
}

char	*pwds_joinfree(char *pwd, char *add)
{
	char	*ret;

	if (!(ret = ft_strjoin(pwd, add)))
		exit(ft_error("malloc failed", 1));
	free(pwd);
	return (ret);
}

char	*pwds_str(t_minishell *m_sh)
{
	char	*pwd;
	char	*tmp;
	t_list	*list;

	if (m_sh->pwd_dslash)
		pwd = ft_strdup("/");
	else
		pwd = NULL;
	list = m_sh->pwds;
//	printf("[pwds_str]firstlist:%p\n", list);
	while (list)
	{
//	printf("[pwds_str]list->content:%s\n", (char *)list->content);
		pwd = pwds_joinfree(pwd, "/");
		pwd = pwds_joinfree(pwd, (char *)list->content);
		list = list->next;
//		printf("[pwds_str]pwd:%s\n", pwd);
	}
	if (!m_sh->pwds)
		pwd = pwds_joinfree(pwd, "/");
//	printf("[pwds_str]pwd:%s\n", pwd);
	return (pwd);
}

int		pwd_update(t_minishell *m_sh, char *argv, int delflag)
{
	char	*pwdval;

	pwdval = value_get("PWD", m_sh);
	free(m_sh->env_oldpwd);
	m_sh->env_oldpwd = NULL;
	if (key_find("OLDPWD", m_sh))
		export_envp(m_sh, ft_strdup("OLDPWD"), pwdval);
	else
		m_sh->env_oldpwd = pwdval;
	pwdlst_update(m_sh, argv, delflag);
	if (delflag == NXCURRENT)
		free(argv);
	pwdval = pwds_str(m_sh);
	errno = 0;
	free(m_sh->env_pwd);
	m_sh->env_pwd = NULL;
	if (key_find("PWD", m_sh))
		export_envp(m_sh, ft_strdup("PWD"), pwdval);
	else
		m_sh->env_pwd = pwdval;
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

int		cd_nx_current(t_minishell *m_sh, char *path, char *argv)
{
	if (*argv == '/')
		return (pwd_update(m_sh, argv, 0));
	else
		return (pwd_update(m_sh, path, NXCURRENT));
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
	if (!**argv)
		return (0);
	if (ft_strlen(*argv) > 255)
		return (ft_cd_error(*argv, 1, "File name too long"));
	if (!**argv)
		return (pwd_update(m_sh, *argv, false));
	if (chdir(*argv))
		return (ft_cd_error(*argv, 1, "No such file or directory"));
	if (!(path = getcwd(NULL, 0)) && (nocurrent = true))
		return (cd_no_current(m_sh, *argv));
	if (nocurrent && !(nocurrent = false))
		return (cd_nx_current(m_sh, path, *argv));
	free(path);
	return (pwd_update(m_sh, *argv, 0));
}
