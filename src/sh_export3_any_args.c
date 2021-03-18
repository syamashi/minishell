/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export3_any_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 17:12:47 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/18 17:35:35 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

void	value_update(t_list *env, char *value, char *key)
{
	if (value)
	{
		free(((t_dict *)env->content)->value);
		((t_dict *)env->content)->value = value;
	}
	free(key);
}

void	pwdshell_export(char *key, char *value, t_minishell *m_sh)
{
	if (!(ft_strncmp(key, "PWD", 4)) && value)
	{
			free(m_sh->env_pwd);
			m_sh->env_pwd = ft_strdup(value);
	}
	if (!(ft_strncmp(key, "OLDPWD", 7)))
	{
		free(m_sh->env_oldpwd);
		m_sh->env_oldpwd = ft_strdup(value);
	}
}

void	dict_addback(t_minishell *m_sh, char *key, char *value)
{
	t_dict	*dict;
	t_list	*new;

	if (!(dict = (t_dict *)malloc(sizeof(t_dict))))
		exit(ft_error("malloc failed", 1, STDERR));
	dict->key = key;
	dict->value = value;
	if (!(new = ft_lstnew(dict)))
		exit(ft_error("malloc failed", 1, STDERR));
	ft_lstadd_back(&m_sh->env_list, new);
}

void	export_envp(t_minishell *m_sh, char *key, char *value)
{
	t_list	*env;

	if (!key)
		return ;
	env = m_sh->env_list;
	if (!(ft_strncmp(key, "PWD", 4)) || !(ft_strncmp(key, "OLDPWD", 7)))
		pwdshell_export(key, value, m_sh);
	while (env)
	{
		if (!ft_strcmp(key, ((t_dict *)env->content)->key))
		{
			value_update(env, value, key);
			return ;
		}
		env = env->next;
	}
	dict_addback(m_sh, key, value);
	return ;
}
