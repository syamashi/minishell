/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_envexpand2_envjoin.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 14:38:25 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/13 12:35:50 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	env_retadd(t_minishell *m_sh, char **new)
{
	char	*value;
	char	*tmp;

	if (!(value = ft_itoa(m_sh->exit_status)))
		exit(ft_error("malloc failed", 1));
	tmp = *new;
	if (!(*new = ft_strjoin(*new, value)))
		exit(ft_error("malloc failed", 1));
	free(tmp);
	tmp = NULL;
	free(value);
	value = NULL;
}

/*
**    $ = $
**  $\\ = $\
**
**  $'' = null,  $"" = null
**  [$][ 0] <- now
**  ["][ 8] <- nexttype = quote
**  [][ 0] <- nextnextline = null
**  ["][ 8]
**
**  "$ $" = $ $ 
**  "$" = $ 
**  ["][ 8] 
**  [][ 0]
**  [$ $ $ $][ 0] <- now
**  ["][ 8] <- nexttype = quote
**          <- nextnexttype = nextnext = NULL
*/

void	empty_key(char **new, t_list *mov)
{
	char	*tmp;

	if (mov->next && (is_quote(((t_pack *)mov->next->content)->type))
		&& mov->next->next && !*(((t_pack*)mov->next->next->content)->line))
		return ;
	tmp = *new;
	if (!(*new = ft_strjoin(*new, "$")))
		exit(ft_error("malloc failed", 1));
	free(tmp);
	tmp = NULL;
}

void	env_add(t_minishell *m_sh, char *key, char **new, t_list *mov)
{
	char	*value;
	char	*tmp;
	int		len;

	len = ft_strlen(key);
	if (!len)
		return (empty_key(new, mov));
	value = value_get(key, m_sh);
	tmp = *new;
	if (!(*new = ft_strjoin(*new, value)))
		exit(ft_error("malloc failed", 1));
	free(value);
	free(tmp);
}

void	env_join(char **new, t_token *t, t_minishell *m_sh, t_list *mov)
{
	char	*key;
	char	*tmp;
	int		len;

	++t->i;
	t->j = t->i;
	if (t->line[t->i] == '?')
		t->i++;
	else
		while (!is_envend(t->line[t->i]))
			(t->i)++;
	if (!(key = ft_substr(t->line, t->j, t->i - t->j)))
		exit(ft_error("malloc failed", 1));
	if (!ft_strncmp(key, "?", 2))
		env_retadd(m_sh, new);
	else
		env_add(m_sh, key, new, mov);
	free(key);
	key = NULL;
	t->j = t->i--;
}
