/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvpe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 11:37:14 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/13 13:23:03 by ewatanab         ###   ########.fr       */
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
	ft_memcpy(buf + (sep - path), "/", 1);
	ft_memcpy(buf + (sep - path) + 1, file, filelen + 1);
	return (0);
}

int			sh_execvpe(const char *file, char *const *argv, char *const *envp, t_minishell *m_sh)
{
	char	*env_path;
	char	*sep;
	char	buf[PATH_MAX + 1];
	bool	eacces;

	if (ft_strchr(file, '/'))
		return (execve(file, argv, envp));
	env_path = value_get("PATH", m_sh);
	sep = env_path;
	eacces = false;
	while (*sep)
	{
		if (!(sep = ft_strchr(env_path, ':')))
			sep = ft_strchr(env_path, 0);
		if (!make_path(buf, env_path, sep, file))
			execve(buf, argv, envp);
		if (errno == EACCES)
			eacces = true;
		env_path = sep + 1;
	}
	if (eacces)
		errno = EACCES;
	if (!*file)
		errno = ENOENT;
	return (-1);
}
