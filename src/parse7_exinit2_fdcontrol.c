/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit2_fdcontrol.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 15:41:55 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/15 15:18:53 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	fdin_set(t_exec **ex, const int n, char *path)
{
	if (errno)
	{
		fd_error(path, 2);
		(*ex)->error_flag = true;
	}
	if ((*ex)->fd_in != 0)
		close((*ex)->fd_in);
	(*ex)->fd_in = n;
}

void	fdout_set(t_exec **ex, const int n, char *path)
{
	if (errno)
	{
		fd_error(path, 2);
		(*ex)->error_flag = true;
	}
	if ((*ex)->fd_out != 1)
		close((*ex)->fd_out);
	(*ex)->fd_out = n;
}

/*
** 1. open ENV
** 2. del ESC
** 3. del QUOTES
** 4. ft_strtrim
** 5. ambiguous check
*/

void	path_make(char **path, char *src, t_minishell *m_sh)
{
	int		i;
	int		j;
	t_list	*packs;
	char	*tmp;

	packs = ft_strtoken(src);
	env_expand(&packs, m_sh);
	quote_del(&packs);
	strs_join(&packs);
	if (!(*path = ft_strdup(((t_pack *)packs->content)->line)))
		exit(ft_error("minishell: malloc failed", 1));
	ft_lstclear(&packs, pack_free);
	tmp = *path;
	*path = ft_strtrim(*path, " \t");
	free(tmp);
	tmp = NULL;
}

void	fd_controller(t_exec **ex, t_list *dir, t_minishell *m_sh)
{
	t_list	*mov;
	int		type;
	char	*path;

	mov = dir;
	while (mov && !(*ex)->error_flag)
	{
		type = ((t_pack *)mov->content)->type;
		mov = mov->next;
		if (ambiguous_check(((t_pack *)mov->content)->line, m_sh, ex))
			break ;
		path_make(&path, ((t_pack *)mov->content)->line, m_sh);
		errno = 0;
		if (type == RDIR)
			fdout_set(ex, open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666), path);
		else if (type == RRDIR)
			fdout_set(
				ex, open(path, O_WRONLY | O_CREAT | O_APPEND, 0666), path);
		else if (type == LDIR)
			fdin_set(ex, open(path, O_RDONLY), path);
		free(path);
		mov = mov->next;
	}
}
