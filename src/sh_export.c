/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 12:01:52 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 19:32:30 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

int		is_keyvalid(char *key)
{
	int i;

	if (ft_isdigit(*key) || !*key)
		return (0);
	i = -1;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
	}
	return (1);
}

int		invalid_key(char *argv, char **key, char *command, int fd)
{
	ft_putstr_fd(MINISHELL, fd);
	ft_putstr_fd(command, fd);
	ft_putstr_fd(": `", fd);
	ft_putstr_fd(argv, fd);
	ft_putstr_fd("': not a valid identifier", fd);
	ft_putstr_fd("\n", fd);
	if (key)
	{
		free(*key);
		*key = NULL;
	}
	return (1);
}

int		sh_export(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;
	char	*key;
	char	*value;
	int		i;
	int		is_invalid;

	argv = exec->argv + 1;
	is_invalid = 0;
	if (!*argv || **argv == '#')
		return (display_export(m_sh, exec));
	while (*argv)
	{
		key = key_get(*argv);
		if (is_keyvalid(key) && (i = -1))
		{
			while (!is_keyend((*argv)[++i]))
				;
			value = (!(*argv)[i]) ? NULL : ft_strdup(*argv + i + 1);
			if (value && i && (*argv)[i] == '=' && (*argv)[i - 1] == '+')
				value = value_add(m_sh, key, value);
			export_envp(m_sh, key, value);
		}
		else
			is_invalid = invalid_key(*argv, &key, "export", exec->fd_err);
		argv++;
	}
	return (is_invalid);
}
