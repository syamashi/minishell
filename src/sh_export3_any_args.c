/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export3_any_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 17:12:47 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/14 22:21:24 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/sh_launch.h"

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

void	export_envp(t_minishell *m_sh, char *key, char *value)
{
	t_list	*env;
	t_list	*new;
	t_dict	*dict;

	env = m_sh->env_list;
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
		exit(ft_error("", 1));
	dict->key = key;
	dict->value = value;
	if (!(new = ft_lstnew(dict)))
		exit(ft_error("", 1));
	envlst_add(&m_sh->env_list, new);
	return ;
}
