/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvpe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 11:37:14 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/16 01:35:19 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

static	int	make_path(char *buf, const char *path, const char *sep,
		const char *file)
{
	size_t	filelen;

	filelen = ft_strlen(file);
	if (filelen + (sep - path) + 1 > PATH_MAX)
		return (-1);
	ft_memcpy(buf, path, sep - path);
	if (sep - path)
		ft_memcpy(buf + (sep - path), "/", 1);
	ft_memcpy(buf + (sep - path) + (sep - path > 0 ? 1 : 0), file, filelen + 1);
	return (0);
}

int			sh_execvpe(const char *file, char *const *argv, char *const *envp, t_minishell *m_sh)
{
	char	*env_path;
	char	*sep;
	char	buf[PATH_MAX + 1];
	int		errno_reserve;

	if (ft_strchr(file, '/'))
		return (execve(file, argv, envp));
	if (!(env_path = value_get("PATH", m_sh)) || !*env_path)
		return (execve(file, argv, envp));
	sep = env_path;
	errno_reserve = 0;
	while (*sep)
	{
		if (!(sep = ft_strchr(env_path, ':')))
			sep = ft_strchr(env_path, 0);
		if (!make_path(buf, env_path, sep, file))
			execve(buf, argv, envp);
		if (errno == EACCES || errno == ENOEXEC)
			errno_reserve = errno;
		env_path = sep + 1;
	}
	if (errno_reserve)
		errno = errno_reserve;
	if (file && !*file)
		errno = ENOENT;
	return (-1);
}
