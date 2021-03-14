/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 12:01:52 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/14 20:56:20 by syamashi         ###   ########.fr       */
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

void	invalid_key(char *argv, int *is_invalid, char **key, char *command)
{
	ft_putstr_fd(MINISHELL, 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(argv, 2);
	ft_putstr_fd("': not a valid identifier", 2);
	ft_putstr_fd("\n", 2);
	*is_invalid = 1;
	if (key)
	{
		free(*key);
		*key = NULL;
	}
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
			invalid_key(*argv, &is_invalid, &key, "export");
		argv++;
	}
	return (is_invalid);
}
