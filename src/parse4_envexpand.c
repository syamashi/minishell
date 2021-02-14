/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_envexpand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 22:00:44 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/14 17:50:08 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

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

void	def_env(t_token *t, char *line, char **new)
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

bool	is_cmdstr(int type, int *pre_type, int *quote_flag)
{
	if (type == SQUOTE && *quote_flag != 2)
		*quote_flag ^= 1;
	if (type == DQUOTE && *quote_flag != 1)
		*quote_flag ^= 2;
	if (*quote_flag == 1 || type != STR)
		return (false);
	if (is_dir(*pre_type))
		return (false);
	return (true);
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
		if (is_cmdstr(type, &pre_type, &quote_flag))
		{
			env_solve(&((t_pack *)mov->content)->line, mov, m_sh);
			if (quote_flag == 0)
				repack(&prev, &mov, packs);
			pre_type = type;
		}
		prev = mov;
		if (is_cmd(type) && mov->next && ((t_pack*)mov->next->content)->type == SPACE)
			pre_type = type;
		mov = mov->next;
	}
}
