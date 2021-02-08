/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvpe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 11:37:14 by ewatanab          #+#    #+#             */
/*   Updated: 2021/01/06 20:32:16 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	make_path(char *buf, const char *path, const char *sep,
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

int		ft_execvpe(const char *file, char *const *argv, char *const *envp)
{
	char	*env_path;
	char	*sep;
	char	buf[PATH_MAX + 1];
	int		eacces;

	if (ft_strchr(file, '/'))
		return (execve(file, argv, envp));
	env_path = ft_strdup("PATH");
	sep = env_path;
	while(!*sep)
	{
		if (!(sep = ft_strchr(env_path, ':')))
			sep = ft_strchr(env_path, 0);
		if (!make_path(buf, env_path, sep, file))
			execve(buf, argv, envp);
		if (errno == EACCES)
			eacces = 1;
		env_path = sep + 1;
	}
	if (eacces)
		errno = EACCES;
	return (-1);
}
