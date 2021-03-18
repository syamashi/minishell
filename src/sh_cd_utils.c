/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 10:53:52 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/18 17:52:56 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

char	*pwds_str(t_minishell *m_sh)
{
	char	*pwd;
	t_list	*list;

	if (m_sh->pwd_dslash)
		pwd = ft_strdup("/");
	else
		pwd = NULL;
	list = m_sh->pwds;
	while (list)
	{
		pwd = pwds_joinfree(pwd, "/");
		pwd = pwds_joinfree(pwd, (char *)list->content);
		list = list->next;
	}
	if (!m_sh->pwds)
		pwd = pwds_joinfree(pwd, "/");
	return (pwd);
}

char	*pwds_joinfree(char *pwd, char *add)
{
	char	*ret;

	if (!(ret = ft_strjoin(pwd, add)))
		exit(ft_error("malloc failed", 1, STDERR));
	free(pwd);
	return (ret);
}

int		pwd_update(t_minishell *m_sh, char *argv, int delflag)
{
	char	*pwdval;

	pwdval = value_get("PWD", m_sh);
	if (!pwdval)
		pwdval = ft_strdup("");
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
