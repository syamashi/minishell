/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export3_any_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 17:12:47 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/13 01:41:53 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

void	envlst_add(t_list **env, t_list *new)
{
	char	*newkey;
	t_list	*mov;
	t_list	*prev;

	mov = *env;
	prev = NULL;
	newkey = ((t_dict *)new->content)->key;
	while (mov)
	{
		if (ft_strcmp(newkey, ((t_dict *)(mov)->content)->key) < 0)
		{
			if (prev)
			{
				prev->next = new;
				new->next = mov;
			}
			else
				ft_lstadd_front(env, new);
			return ;
		}
		prev = mov;
		mov = mov->next;
	}
	ft_lstadd_back(env, new);
}

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
	if (!(ft_strncmp(key, "PWD", 4)))
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

void	export_envp(t_minishell *m_sh, char *key, char *value)
{
	t_list	*env;
	t_list	*new;
	t_dict	*dict;

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
	if (!(dict = (t_dict *)malloc(sizeof(t_dict))))
		exit(ft_error("malloc failed", 1));
	dict->key = key;
	dict->value = value;
	if (!(new = ft_lstnew(dict)))
		exit(ft_error("malloc failed", 1));
	envlst_add(&m_sh->env_list, new);
	return ;
}
