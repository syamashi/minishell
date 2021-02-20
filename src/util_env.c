/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:48:45 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/20 15:02:27 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

/*
** $abc
** start read "abc"
*/

char	*key_get(char *line)
{
	int i;

	i = 0;
	while (!is_keyend(line[i]))
		i++;
	if (i && line[i] == '=' && line[i - 1] == '+')
		i--;
	return (ft_substr(line, 0, i));
}

/*
**  $option is not supported.
**  $$ = $null + $null
*/

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
		len = ft_strlen(key);
		if (len == 0)
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

bool	key_find(char *key, t_minishell *m_sh)
{
	t_list	*mov;

	mov = m_sh->env_list;
	while (mov)
	{
		if (!ft_strcmp(key, ((t_dict*)mov->content)->key))
			return (true);
		mov = mov->next;
	}
	return (false);
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
			exit(ft_error("", 1));
		free(tmp);
		tmp = NULL;
	}
	free(add);
	add = NULL;
	return (value);
}
