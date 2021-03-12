/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_envexpand2_envjoin.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 14:38:25 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/12 15:31:44 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	env_retadd(t_minishell *m_sh, char **new)
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

	if (mov->next && ((t_pack *)mov->next->content)->type == SQUOTE)
		return ;
	tmp = *new;
	if (!(*new = ft_strjoin(*new, "$")))
		exit(ft_error("minishell: malloc failed", 1));
	free(tmp);
	tmp = NULL;
}

void	env_add(t_minishell *m_sh, char *key, char **new)
{
	char	*value;
	char	*tmp;

	value = value_get(key, m_sh);
	tmp = *new;
	if (!(*new = ft_strjoin(*new, value)))
		exit(ft_error("minishell: malloc failed", 1));
	free(tmp);
}

void	env_join(char **new, t_token *t, t_minishell *m_sh)
{
	char	*key;
	char	*tmp;
	int		len;

	++t->i;
	t->j = t->i;
	if (t->line[t->i] == '?')
		t->i++;
	else
		while (!is_keyend(t->line[t->i]))
			(t->i)++;
	if (!(key = ft_substr(t->line, t->j, t->i - t->j)))
		exit(ft_error("minishell: malloc failed", 1));
	if (!ft_strncmp(key, "?", 2))
		env_retadd(m_sh, new);
	else
		env_add(m_sh, key, new);
	free(key);
	key = NULL;
	t->j = t->i--;
}
