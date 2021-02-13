/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 12:01:52 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/13 14:16:07 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/sh_launch.h"

int	display_export(t_minishell *m_sh, int n)
{
	return (n);
}

int	is_keyvalid(char *key)
{
	return (1);
}

void	invalid_key(char *argv, int *error_flag)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(argv, 2);
	ft_putstr_fd("': not a valid identifier", 2);
	ft_putstr_fd("\n", 2);
	*error_flag |= 1;
}

void	export_envp(t_minishell *m_sh, char *key, char *value)
{
	return;
}

int	sh_export(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;
	char	*key;
	char	*value;
	int		error_flag;
	int		i;

	argv = exec->argv + 1;
	error_flag = 0;
	if (!(argv))
		return (display_export(m_sh, 0));
	while (*argv)
	{
		key = key_get(*argv);
		if (is_keyvalid(key))
		{
			i = -1;
			while (!is_keyend((*argv)[++i]))
				;
			if (!(*argv)[i])
				value = NULL;
			else
				if (!(value = ft_strdup(*argv + i + 1)))
					exit(ft_error("", 1));
			printf("[sh_export] key:[%s], value[%s], i:%d\n", key, value, i);
			export_envp(m_sh, key, value);
		}
		else
			invalid_key(*argv, &error_flag);
		argv++;
	}
	return (error_flag);
}
