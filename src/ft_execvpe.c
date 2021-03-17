/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvpe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 11:37:14 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/17 19:26:03 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

static	int	make_path(char *buf, const char *path, const char *sep,
		const char *file)
{
	size_t		filelen;
	struct stat	sb;

	filelen = ft_strlen(file);
	if (filelen + (sep - path) + 1 > PATH_MAX)
		return (-1);
	ft_memcpy(buf, path, sep - path);
	if (sep - path)
	{
		ft_memcpy(buf + (sep - path), 0, 1);
		if (!stat(buf, &sb) && !S_ISDIR(sb.st_mode))
		{
			errno = ENOTDIR;
			return (-1);
		}
		ft_memcpy(buf + (sep - path), "/", 1);
	}
	ft_memcpy(buf + (sep - path) + (sep - path > 0 ? 1 : 0), file, filelen + 1);
	return (0);
}

void		internal_error(const char *path, char *str, int ret)
{
	ft_putstr_fd(MINISHELL, STDERR);
	ft_putstr_fd((char *)path, STDERR);
	ft_putstr_fd(": ", STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putstr_fd("\n", STDERR);
	exit(ret);
}

void		error_branch(int mode, const char *file)
{
	struct stat sb;

	if (errno == EACCES && mode != 0)
	{
		if (stat(file, &sb) == 0 && S_ISDIR(sb.st_mode))
			internal_error(file, "is a directory", 126);
		internal_error(file, "Permission denied", 126);
	}
	if (errno == ENOENT && mode != 0)
		internal_error(file, strerror(ENOENT), 127);
	if (errno == ENOEXEC)
		exit(0);
	if (errno == ENOTDIR)
		internal_error(file, strerror(ENOTDIR), 126);
	if (errno == EISDIR)
		internal_error(file, "is a directory", 126);
	if (errno == EACCES)
		internal_error(file, "Permission denied", 126);
	internal_error(file, "command not found", 127);
}

void		simple_case(const char *file, char *const *argv, char *const *envp)
{
	if (!*file)
		error_branch(0, file);
	if (ft_strchr(file, '/'))
		error_branch(execve(file, argv, envp), file);
}

void		search_and_exec(const char *file, char *const *argv
						, char *const *envp, t_minishell *m_sh)
{
	char		*env_path;
	char		*sep;
	char		buf[PATH_MAX + 1];
	int			errno_reserve;
	struct stat	sb;

	simple_case(file, argv, envp);
	if (!(env_path = value_get("PATH", m_sh)) || !*env_path)
		error_branch(execve(file, argv, envp), file);
	sep = env_path;
	errno_reserve = ENOENT;
	while (*sep)
	{
		if (!(sep = ft_strchr(env_path, ':')))
			sep = ft_strchr(env_path, 0);
		if (!make_path(buf, env_path, sep, file) && !stat(buf, &sb))
			execve(buf, argv, envp);
		if (errno == EACCES || errno == ENOEXEC || errno == ENOTDIR)
			errno_reserve = errno;
		if (errno == EACCES && S_ISDIR(sb.st_mode))
			errno_reserve = EISDIR;
		env_path = sep + 1;
	}
	errno = errno_reserve;
	error_branch(0, file);
}
