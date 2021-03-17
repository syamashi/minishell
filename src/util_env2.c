/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_env2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 20:57:23 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/16 21:26:52 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

/*
** char	*key_get(char *line)
**
** ex [$abc]
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
