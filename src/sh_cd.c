/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:23:23 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/17 22:25:07 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

int		cd_no_args(t_minishell *m_sh, int fd)
{
	char	*home;
	char	*path;

	if (!(home = value_get("HOME", m_sh)))
		return (cd_error(NULL, 1, "HOME not set", fd));
	if (!*home && (path = getcwd(NULL, 0)))
	{
		free(home);
		return (pwd_update(m_sh, path, false));
	}
	if (chdir(home) == -1)
		return (ft_cd_free_error(home, 127, "No such file or directory", fd));
	ft_lstclear(&m_sh->pwds, free);
	pwd_update(m_sh, home, false);
	free(home);
	return (0);
}

int		cd_no_current(t_minishell *m_sh, char *argv, int fd)
{
	pwd_update(m_sh, argv, NOCURRENT);
	ft_putstr_fd("cd: error retrieving current directory: ", fd);
	ft_putstr_fd("getcwd: cannot access parent directories : ", fd);
	ft_putstr_fd("No such file or directory\n", fd);
	return (0);
}

int		cd_nx_current(t_minishell *m_sh, char *path, char *argv)
{
	if (*argv == '/')
		return (pwd_update(m_sh, argv, 0));
	else
		return (pwd_update(m_sh, path, NXCURRENT));
}

int		cd_blank_args(t_minishell *m_sh, char *path, char *argv, int fd)
{
	char	*input_pwd;

	free(path);
	input_pwd = pwds_str(m_sh);
	if (chdir(input_pwd))
	{
		free(input_pwd);
		return (cd_error(argv, 1, "No such file or directory", fd));
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
		return (cd_no_args(m_sh, exec->fd_err));
	if (ft_strlen(*argv) > 255)
		return (cd_error(*argv, 1, "File name too long", exec->fd_err));
	if (!**argv && (path = getcwd(NULL, 0)))
		return (cd_blank_args(m_sh, path, *argv, exec->fd_err));
	if (chdir(*argv))
		return (cd_error(*argv, 1, "No such file or directory", exec->fd_err));
	path = getcwd(NULL, 0);
	if (!path && (nocurrent = true))
		return (cd_no_current(m_sh, *argv, exec->fd_err));
	if (nocurrent && !(nocurrent = false))
		return (cd_nx_current(m_sh, path, *argv));
	if (is_linkdel(m_sh, *argv))
		return (pwd_update(m_sh, path, NXCURRENT));
	free(path);
	return (pwd_update(m_sh, *argv, 0));
}
