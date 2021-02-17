/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit2_fdcontrol.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 15:41:55 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/17 18:17:35 by syamashi         ###   ########.fr       */
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

bool	ambiguous_error(t_minishell *m_sh, char *str, t_exec **ex)
{
	m_sh->exit_status = dir_error(str, 1);
	(*ex)->error_flag = true;
	fdin_set(ex, 0, "");
	fdout_set(ex, 1, "");
	return (true);
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
		if (!(path = path_make(((t_pack *)mov->content)->line, m_sh)))
			if (ambiguous_error(m_sh, ((t_pack *)mov->content)->line, ex))
				break ;
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
