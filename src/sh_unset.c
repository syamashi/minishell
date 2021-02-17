/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 15:47:16 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/17 18:33:21 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

void	envlst_delone(t_minishell *m_sh, t_list *mov, t_list *prev)
{
	t_list	*tmp;

	if (!prev)
	{
		tmp = m_sh->env_list->next;
		ft_lstdelone(m_sh->env_list, env_free);
		m_sh->env_list = tmp;
	}
	else
	{
		tmp = mov->next;
		ft_lstdelone(mov, env_free);
		prev->next = tmp;
	}
}

void	unset_envp(t_minishell *m_sh, char *key)
{
	char	*dkey;
	t_list	*mov;
	t_list	*prev;
	t_list	*tmp;

	mov = m_sh->env_list;
	prev = NULL;
	while (mov)
	{
		dkey = ((t_dict *)mov->content)->key;
		if (!ft_strcmp(key, dkey))
		{
			envlst_delone(m_sh, mov, prev);
			return ;
		}
		prev = mov;
		mov = mov->next;
	}
}

int		sh_unset(t_minishell *m_sh, t_exec *exec)
{
	char	**key;
	int		i;
	int		is_invalid;

	key = exec->argv + 1;
	is_invalid = 0;
	if (!*key)
		return (0);
	while (*key)
	{
		if (is_keyvalid(*key))
			unset_envp(m_sh, *key);
		else
			invalid_key(*key, &is_invalid, NULL);
		key++;
	}
	return (is_invalid);
}
