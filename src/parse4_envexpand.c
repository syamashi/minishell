/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_envexpand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 22:00:44 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/11 21:20:35 by syamashi         ###   ########.fr       */
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

void	pre_join(char **new, t_token *t)
{
	char	*tmp;
	char	*add;

	if (t->i == t->j)
		return ;
	if (!(add = ft_substr(t->line, t->j, t->i - t->j)))
		exit(ft_error("minishell: malloc failed", 1));
	tmp = *new;
	if (!(*new = ft_strjoin(*new, add)))
		exit(ft_error("minishell: malloc failed", 1));
	free(tmp);
	free(add);
}

static void	def_env(t_token *t, char *line, char **new)
{
	t->i = -1;
	t->j = 0;
	t->line = line;
	if (!(*new = ft_strdup("")))
		exit(ft_error("minishell: malloc failed", 1));
}

void	env_solve(char **line, t_list *mov, t_minishell *m_sh)
{
	char	*new;
	t_token t;

	def_env(&t, *line, &new);
	while ((*line)[++t.i])
	{
		if ((*line)[t.i] != '$')
			continue;
		pre_join(&new, &t);
		env_join(&new, mov, &t, m_sh);
	}
	if (t.j != t.i)
		pre_join(&new, &t);
	if (line)
		free(*line);
	*line = new;
}

bool	is_skip(int type, int *pre_type, int *quote_flag)
{
	if (type == SQUOTE)
		*quote_flag ^= 1;
	if (type == DQUOTE)
		*quote_flag ^= 2;
	if (*quote_flag == 1 || type != STR)
		return (true);
	if (is_dir(*pre_type))
		return (true);
	return (false);
}

t_list *space_strtoken(char *line)
{
	t_token	t;
	t_list	*list;
	t_pack	*pack;

	def_strtoken(&t, &list, &line, &pack); //pack作る
	while (line[++t.i])
	{
		if (is_space(line[t.i]))
			pack_metaadd(&pack, &list, " ", SPACE);
		while (is_space(line[t.i]) && line[t.i])
			t.i++;
		t.j = t.i;
		while (!is_space(line[t.i]) && line[t.i])
			t.i++;
		if (t.j != t.i)
			pack_join(&pack, line + t.j, t.i - t.j);
		if (*pack->line)
			pack_add(&pack, &list, STR);
		--t.i;
	}
	if (t.i == 0)
		pack_add(&pack, &list, STR);
	return (pack_end(&pack, &list));
}

void repack(t_list **prev, t_list **mov, t_list **packs)
{
	t_list	*new;
	t_list	*tmp;
	t_list	*newlast;
	char	*line;

	line = ((t_pack *)(*mov)->content)->line;
	new = space_strtoken(line);
	newlast = ft_lstlast(new);
	if (*prev)
		(*prev)->next = new;
	else
		*packs = new;
	ft_lstadd_back(&new, (*mov)->next);
	ft_lstdelone(*mov, pack_free);
	if (newlast)
		*mov = newlast;
	else
		*mov = new;
}

/*
**  only below will expand
**  1. STR
**  2. pre_type != DIRS
**  3. NOT SQUOTE
*/

void	env_expand(t_list **packs, t_minishell *m_sh)
{
	t_list	*mov;
	t_list	*prev;
	int		type;
	int		pre_type;
	int		quote_flag;

	quote_flag = 0;
	pre_type = -1;
	prev = NULL;
	mov = *packs;
	while (mov)
	{
		type = ((t_pack *)mov->content)->type;
		if (!is_skip(type, &pre_type, &quote_flag))
		{
			env_solve(&((t_pack *)mov->content)->line, mov, m_sh);
			if (quote_flag != 2)
				repack(&prev, &mov, packs);
			pre_type = type;
		}
		prev = mov;
		if (mov->next && ((t_pack*)mov->next->content)->type == SPACE)
			pre_type = ((t_pack *)mov->content)->type;
		mov = mov->next;
	}
}
