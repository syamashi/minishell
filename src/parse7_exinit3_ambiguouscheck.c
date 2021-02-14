/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit3_ambiguouscheck.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 16:04:30 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/13 02:09:14 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

bool	envcheck_solve(char **value)
{
	char	*tmp;
	int		i;

	tmp = *value;
	if (!(*value = ft_strtrim(*value, " \t")))
		exit(ft_error("minishell: malloc failed", 1));
	free(tmp);
	i = -1;
	while ((*value)[++i])
		if ((*value)[i] == ' ')
			return (true);
	return (false);
}

bool	redirect_envcheck(char *line, t_minishell *m_sh)
{
	int		i;
	char	*key;
	char	*value;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '$')
		{
			if (!(key = key_get(line + i + 1)))
				exit(ft_error("minishell: malloc failed", 1));
			if (!(value = value_get(key, m_sh)))
				exit(ft_error("minishell: malloc failed", 1));
			free(key);
			if (envcheck_solve(&value))
			{
				free(value);
				return (true);
			}
			free(value);
		}
	}
	return (false);
}

bool	ambiguous_error(t_minishell *m_sh, char *str, t_exec **ex)
{
	m_sh->exit_status = dir_error(str, 1);
	(*ex)->error_flag = true;
	fdin_set(ex, 0, "");
	fdout_set(ex, 1, "");
	return (true);
}

bool	ambiguous_check(char *str, t_minishell *m_sh, t_exec **ex)
{
	t_list	*packs;
	t_list	*mov;
	char	*line;
	int		type;
	int		quote_flag;

	packs = ft_strtoken(str);
	mov = packs;
	quote_flag = 0;
	while (mov)
	{
		packinfo_get(&line, &type, mov);
		if (type == SQUOTE && quote_flag != 2)
			quote_flag ^= 1;
		if (type == DQUOTE && quote_flag != 1)
			quote_flag ^= 2;
		if (type == STR && !quote_flag && redirect_envcheck(line, m_sh))
		{
			ft_lstclear(&packs, pack_free);
			return (ambiguous_error(m_sh, str, ex));
		}
		mov = mov->next;
	}
	ft_lstclear(&packs, pack_free);
	return (false);
}
