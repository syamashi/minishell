/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_envexpand2_envjoin.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 14:38:25 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/12 14:52:47 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void env_retadd(t_minishell *m_sh, char **new)
{
	char	*value;
	char	*tmp;

	if (!(value = ft_itoa(m_sh->exit_status)))
		exit(ft_error("minishell: malloc failed", 1));
	tmp = *new;
	if (!(*new = ft_strjoin(*new, value)))
		exit(ft_error("minishell: malloc failed", 1));
	free(tmp);
	tmp = NULL;
	free(value);
	value = NULL;
}

/*
**    $ = $
**  $'' = null
**  $"" = null
**  $\\ = $\ 
*/

void	empty_key(char **new, t_list *mov)
{
	char	*tmp;

	if (mov->next && is_quote(((t_pack *)mov->next->content)->type))
		return ;
	tmp = *new;
	if (!(*new = ft_strjoin(*new, "$")))
		exit(ft_error("minishell: malloc failed", 1));
	free(tmp);
	tmp = NULL;
}

void env_add(t_minishell *m_sh, t_list *mov, char *key, char **new)
{
	t_list	*pos;
	char	*tmp;
	int		len;
	char	*dkey;
	char	*value;

	len = ft_strlen(key);
	if (len == 0)
		return (empty_key(new, mov));
	pos = m_sh->env_list;
	while (pos)
	{
		dkey = ((t_dict *)pos->content)->key;
		if (!ft_strncmp(key, dkey, len + 1))
		{
			value = ((t_dict *)pos->content)->value;
			tmp = *new;
			if (!(*new = ft_strjoin(*new, value)))
				exit(ft_error("[env_join] new malloc error", 1));
			free(tmp);
			break;
		}
		pos = pos->next;
	}
}

void	env_join(char **new, t_list *mov, t_token *t, t_minishell *m_sh)
{
	char	*key;
	char	*tmp;
	int		len;

	++t->i;
	t->j = t->i;
	while (!is_keyend(t->line[t->i]))
		(t->i)++;
	if (!(key = ft_substr(t->line, t->j, t->i - t->j)))
		exit(ft_error("minishell: malloc failed", 1));
	if (!ft_strncmp(key, "?", 2))
		env_retadd(m_sh, new);
	else
		env_add(m_sh, mov, key, new);
	free(key);
	key = NULL;
	t->j = t->i--;
}
