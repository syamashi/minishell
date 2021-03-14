/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:23:23 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/14 18:42:03 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

int		cd_no_args(t_minishell *m_sh)
{
	char	*home;
	char	*path;

	if (!(home = value_get("HOME", m_sh)))
		return (ft_cd_error(NULL, 1, "HOME not set"));
	if (!*home && (path = getcwd(NULL, 0)))
		return (pwd_update(m_sh, path, false));
	if (chdir(home) == -1)
	{
		free(home);
		return (ft_cd_error(home, 1, "No such file or directory"));
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

int		cd_blank_args(t_minishell *m_sh, char *path, char *argv)
{
	char	*input_pwd;

	free(path);
	input_pwd = pwds_str(m_sh);
	if (chdir(input_pwd))
	{
		free(input_pwd);
		return (ft_cd_error(argv, 1, "No such file or directory"));
	}
	free(input_pwd);
	return (pwd_update(m_sh, argv, false));
}

int		sh_cd(t_minishell *m_sh, t_exec *exec)
{
	char			**argv;
	char			*path;
	static	bool	nocurrent;

	errno = 0;
	argv = exec->argv + 1;
	if (!*argv)
		return (cd_no_args(m_sh));
	if (*(argv + 1))
		return (ft_cd_error(NULL, 1, "too many arguments"));
	if (ft_strlen(*argv) > 255)
		return (ft_cd_error(*argv, 1, "File name too long"));
	if (!**argv && (path = getcwd(NULL, 0)))
		return (cd_blank_args(m_sh, path, *argv));
	if (chdir(*argv))
		return (ft_cd_error(*argv, 1, "No such file or directory"));
	if (!(path = getcwd(NULL, 0)) && (nocurrent = true))
		return (cd_no_current(m_sh, *argv));
	if (nocurrent && !(nocurrent = false))
		return (cd_nx_current(m_sh, path, *argv));
	if (is_linkdel(m_sh, *argv))
		return (pwd_update(m_sh, path, NXCURRENT));
	free(path);
	return (pwd_update(m_sh, *argv, 0));
}
