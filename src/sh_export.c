/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 12:01:52 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/14 17:26:23 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/sh_launch.h"

int		is_keyvalid(char *key)
{
	int i;

	if (ft_isdigit(*key))
		return (0);
	i = -1;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
	}
	return (1);
}

void	invalid_key(char *argv, t_minishell *m_sh, char **key)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(argv, 2);
	ft_putstr_fd("': not a valid identifier", 2);
	ft_putstr_fd("\n", 2);
	m_sh->exit_status = 1;
	free(*key);
	*key = NULL;
}

int		sh_export(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;
	char	*key;
	char	*value;
	int		i;

	argv = exec->argv + 1;
	if (!*argv)
		return (display_export(m_sh, 0));
	while (*argv)
	{
		key = key_get(*argv);
		if (is_keyvalid(key))
		{
			i = -1;
			while (!is_keyend((*argv)[++i]))
				;
			value = (!(*argv)[i]) ? NULL : ft_strdup(*argv + i + 1);
			export_envp(m_sh, key, value);
		}
		else
			invalid_key(*argv, m_sh, &key);
		argv++;
	}
	return (0);
}
