/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:23:23 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/18 18:50:42 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

/*
** export_envp() required malloced key/value and freed
*/

int		pwd_update(t_minishell *m_sh)
{
	char	*value;
	char	*buf;
	char	*pwdval;

	buf = NULL;
	if (!(value = value_get("PWD", m_sh)))
		if (!(value = ft_strdup("")))
			exit(ft_error("minishell: malloc failed", 1));
	if (key_find("OLDPWD", m_sh))
		export_envp(m_sh, ft_strdup("OLDPWD"), value);
	else
		free(value);
	if (key_find("PWD", m_sh))
	{
		errno = 0;
		if (!(pwdval = getcwd(buf, 1024)))
			return (ft_error(strerror(errno), 1));
		export_envp(m_sh, ft_strdup("PWD"), pwdval);
	}
	return (0);
}

int		cd_no_args(t_minishell *m_sh)
{
	char	*home;

	if (!(home = value_get("HOME", m_sh)))
		return (ft_error("minishell: cd: HOME not set", 1));
	if (chdir(home) == -1)
	{
		free(home);
		return (ft_cd_error(home, 1));
	}
	free(home);
	return (0);
}

int		sh_cd(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;
	char	*pwd;
	char	*buf;

	buf = NULL;
	errno = 0;
	argv = exec->argv + 1;
	if (!*argv)
		return (cd_no_args(m_sh));
	if (!**argv)
		return (pwd_update(m_sh));
	if (chdir(*argv))
		return (ft_cd_error(*argv, 1));
	return (pwd_update(m_sh));
}
