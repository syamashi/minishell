/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit4_fdget.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 21:58:44 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/17 19:44:44 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	fd_rdir(t_exec **ex, char *path, int rint)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (errno && ((*ex)->error_flag = true))
		return ;
	if (rint == -2)
	{
		close(fd);
		return ;
	}
	rint = (rint == -1) ? 1 : rint;
	if (rint == 1)
	{
		if ((*ex)->fd_out != 1)
			close((*ex)->fd_out);
		(*ex)->fd_out = fd;
	}
	if (rint == 2)
	{
		if ((*ex)->fd_err != 2)
			close((*ex)->fd_err);
		(*ex)->fd_err = fd;
	}
}

void	fd_rrdir(t_exec **ex, char *path, int rint)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (errno && ((*ex)->error_flag = true))
		return ;
	if (rint == -2)
	{
		close(fd);
		return ;
	}
	rint = (rint == -1) ? 1 : rint;
	if (rint == 1)
	{
		if ((*ex)->fd_out != 1)
			close((*ex)->fd_out);
		(*ex)->fd_out = fd;
	}
	if (rint == 2)
	{
		if ((*ex)->fd_err != 2)
			close((*ex)->fd_err);
		(*ex)->fd_err = fd;
	}
}

void	fd_ldir(t_exec **ex, char *path, int rint)
{
	int	fd;

	errno = 0;
	fd = open(path, O_RDONLY);
	if (errno && ((*ex)->error_flag = true))
		return ;
	if (rint == -2)
	{
		close(fd);
		return ;
	}
	rint = (rint == -1) ? 0 : rint;
	if ((*ex)->fd_in != 0)
		close((*ex)->fd_in);
	(*ex)->fd_in = fd;
}

void	fd_get(t_exec **ex, char *path, int type, int rint)
{
	errno = 0;
	if (type == RDIR)
		fd_rdir(ex, path, rint);
	if (type == RRDIR)
		fd_rrdir(ex, path, rint);
	if (type == LDIR)
		fd_ldir(ex, path, rint);
	if (errno)
		(*ex)->error_flag = true;
}
