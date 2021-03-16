/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:48:45 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/16 21:27:05 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

bool	pwdshell_exist(char *key, t_minishell *m_sh)
{
	if (!ft_strncmp(key, "PWD", 4) && m_sh->env_pwd)
		return (true);
	if (!ft_strncmp(key, "OLDPWD", 7) && m_sh->env_oldpwd)
		return (true);
	return (false);
}

char	*pwdshell_dup(char *key, t_minishell *m_sh)
{
	char	*str;

	if (!ft_strncmp(key, "PWD", 4))
		return (ft_strdup(m_sh->env_pwd));
	if (!ft_strncmp(key, "OLDPWD", 7))
		return (ft_strdup(m_sh->env_oldpwd));
	return (NULL);
}

char	*value_get(char *key, t_minishell *m_sh)
{
	int		len;
	t_list	*mov;
	char	*dkey;

	if (!key)
		return (NULL);
	if (!ft_strncmp(key, "?", 2))
		return (ft_itoa(m_sh->exit_status));
	else
	{
		if (pwdshell_exist(key, m_sh))
			return (pwdshell_dup(key, m_sh));
		if (!(len = ft_strlen(key)))
			return (ft_strdup(""));
		mov = m_sh->env_list;
		while (mov)
		{
			dkey = ((t_dict *)mov->content)->key;
			if (!ft_strncmp(key, dkey, len + 1))
				return (ft_strdup(((t_dict *)mov->content)->value));
			mov = mov->next;
		}
	}
	return (NULL);
}

char	*value_add(t_minishell *m_sh, char *key, char *value)
{
	char	*add;
	char	*tmp;

	add = value_get(key, m_sh);
	if (add)
	{
		tmp = value;
		if (!(value = ft_strjoin(add, value)))
			exit(ft_error("malloc failed", 1, STDERR));
		free(tmp);
		tmp = NULL;
	}
	free(add);
	add = NULL;
	return (value);
}
