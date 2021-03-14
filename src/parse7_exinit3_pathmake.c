/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit3_pathmake.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 16:04:30 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 02:05:11 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

static	void	solve_dummied(char **line)
{
	unsigned char	uc;
	int				i;

	i = -1;
	uc = DUMMY;
	while ((*line)[++i])
	{
		if (is_space((*line)[i]))
			(*line)[i] = uc;
	}
}

static	void	envspace_dummied(t_list **packs)
{
	char	*line;
	int		type;
	t_list	*mov;
	int		quote_flag;

	mov = *packs;
	quote_flag = 0;
	while (mov)
	{
		packinfo_get(&line, &type, mov);
		quoteflag_get(type, &quote_flag);
		if (!quote_flag && type == STR)
			solve_dummied(&line);
		mov = mov->next;
	}
}

static	char	*dummy_set(void)
{
	unsigned char	*set;

	if (!(set = (unsigned char *)malloc(2)))
		exit(ft_error("malloc failed", 1));
	set[0] = DUMMY;
	set[1] = '\0';
	return ((char*)set);
}

static	bool	ambiguous_check(char **path)
{
	int i;

	i = -1;
	if (!*path)
		return (true);
	while ((*path)[++i])
	{
		if ((unsigned char)(*path)[i] == DUMMY)
		{
			free(*path);
			*path = NULL;
			return (true);
		}
	}
	return (false);
}

char			*path_make(char *src, t_minishell *m_sh)
{
	t_list	*packs;
	char	*set;
	char	*path;

	packs = ft_strtoken(src);
	env_expand(&packs, m_sh, 1);
	null_del(&packs);
	if (!packs)
		return (NULL);
	envspace_dummied(&packs);
	quote_del(&packs);
	strs_join(&packs);
	if (!(path = ft_strdup(((t_pack *)packs->content)->line)))
		exit(ft_error("malloc failed", 1));
	ft_lstclear(&packs, pack_free);
	set = dummy_set();
	path = simpletrim(path, set);
	free(set);
	if (ambiguous_check(&path))
	{
		free(path);
		return (NULL);
	}
	return (path);
}
