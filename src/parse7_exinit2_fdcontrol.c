/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit2_fdcontrol.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 15:41:55 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 14:58:41 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

/*void	fdin_set(t_exec **ex, const int n, char *path)
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
*/
bool	ambiguous_error(t_minishell *m_sh, char *str, t_exec **ex)
{
	errno = 0;

	m_sh->exit_status = dir_error(str, 1);
	(*ex)->error_flag = true;
	return (true);
}

int	rint_atoi(const char *nptr)
{
	char		*str;
	int			n;
	int			num;
	long long	m;

	str = (char *)nptr;
	while (*str == ' ' || *str == '\t' ||
	*str == '\n' || *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	n = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-')
		str++;
	m = 0;
	while (ft_isdigit(*str))
	{
		num = (*str++ - '0');
		if (m > 2147483647)
			return (-1);
		m = m * 10 + n * num;
	}
	return (m);
}

void	rint_error(char *rint, int rint_num, t_minishell *m_sh)
{
	ft_putstr_fd(MINISHELL, STDERR);
	if (rint_num == -1)
		ft_putstr_fd("file descriptor out of range", STDERR);
	if (rint_num > 255)
		ft_putstr_fd(rint, STDERR);
	ft_putstr_fd(": Bad file descriptor\n", STDERR);
	m_sh->exit_status = 1;
}

/*
**  ex [2>file]
**
**  int fd = open();
**  int backup = dup(rint);
**  dup2(fd, rint);
**  close(fd)
**  ---backup---
**  dup2(backup, rint);
*/

void	fd_rdir(t_minishell *m_sh, char *path, int rint)
{
	int			fd;
	int			backup;
	t_redirect	*rinfo;
	t_list		*new;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (rint == -1)
		rint = 1;
	if (rint == -2)
	{
		close(fd);
		return ;
	}
	backup = dup(rint);
	printf("[fd_rdir]fd:%d, rint:%d\n", fd, rint);
	dup2(fd, rint);
	close(fd);
	if (!(rinfo = (t_redirect *)malloc(sizeof(t_redirect))))
		exit(ft_error("", 1));
	rinfo->rint = rint;
	rinfo->backup = backup;
	if (!(new = ft_lstnew(rinfo)))
		exit(ft_error("", 1));
	ft_lstadd_front(&m_sh->fd_backup, new);
}

void	fd_rrdir(t_minishell *m_sh, char *path, int rint)
{
	int			fd;
	int			backup;
	t_redirect	*rinfo;
	t_list		*new;

	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (rint == -1)
		rint = 1;
	if (rint == -2)
	{
		close(fd);
		return ;
	}
	backup = dup(rint);
	dup2(fd, rint);
		close(fd);
	if (!(rinfo = (t_redirect *)malloc(sizeof(t_redirect))))
		exit(ft_error("", 1));
	rinfo->rint = rint;
	rinfo->backup = backup;
	if (!(new = ft_lstnew(rinfo)))
		exit(ft_error("", 1));
	ft_lstadd_front(&m_sh->fd_backup, new);
}

void	fd_ldir(t_minishell *m_sh, char *path, int rint)
{
	int			fd;
	int			backup;
	t_redirect	*rinfo;
	t_list		*new;

	fd = open(path, O_RDONLY);
	if (rint == -1)
		rint = 0;
	if (rint == -2)
	{
		close(fd);
		return ;
	}
	backup = dup(rint);
	dup2(fd, rint);
		close(fd);
	if (!(rinfo = (t_redirect *)malloc(sizeof(t_redirect))))
		exit(ft_error("", 1));
	rinfo->rint = rint;
	rinfo->backup = backup;
	if (!(new = ft_lstnew(rinfo)))
		exit(ft_error("", 1));
	ft_lstadd_front(&m_sh->fd_backup, new);
}

/*  fd_get
**  
**  
*/

void	fd_get(t_minishell *m_sh, char *path, int type, int	rint)
{
	errno = 0;
	if (type == RDIR)
		fd_rdir(m_sh, path, rint);
	if (type == RRDIR)
		fd_rrdir(m_sh, path, rint);
	if (type == LDIR)
		fd_ldir(m_sh, path, rint);
}

/*void	solve_rint(int fd, char *rint, t_exec **ex, t_minishell *m_sh)
{
	int			rint_num;
	t_list		*new;
	t_redirect	*rfd;

	printf("[solve_rint] \n");
	if (!rint || (*ex)->error_flag)
		return ;
	rint_num = rint_atoi(rint);
	if (rint_num == -1 || rint_num > 255)
	{
		rint_error(rint, rint_num);
		(*ex)->error_flag = true;
		printf("[solve_rint]error\n");
		return;
	}
	if (!(rfd = (t_redint *)malloc(sizeof(t_redint))))
		exit(ft_error("malloc failed", 1));
	rfd->rint = rint_num;
	rfd->backup = dup(rint_num);
	if (!(new = ft_lstnew(rfd)))
		exit(ft_error("malloc failed", 1));
	ft_lstadd_front(&m_sh->fd_backup, new);
	printf("dup2(%d, %d), size:%d\n", fd, rint_num, ft_lstsize(m_sh->fd_backup));
	if (dup2(fd, rint_num) < 0)
	{
		ft_error("minishell", 1);
		exit(1);
	}
	close(fd);
}

void	fd_reget(int fd, char *rint, t_exec **ex, int type)
{
	if (!rint || (*ex)->error_flag)
		return ;
	if (type == LDIR)
		(*ex)->fd_in = rint_atoi(rint);
	if (type == RDIR || type == RRDIR)
		(*ex)->fd_out = rint_atoi(rint);
}
*/

/*
**  bash-3.2$ echo a -256>file
**  bash-3.2$ cat file
**  a -256
**
**  bash-3.2$ echo aaa > file
**  bash-3.2$ echo a 12340918349018904>file
**  bash: file descriptor out of range: Bad file descriptor
**  bash-3.2$ cat file
**  
**  default rint = -1 
**  error rint = -2 
*/

void	fd_controller(t_exec **ex, t_list *dir, t_minishell *m_sh)
{
	t_list	*mov;
	int		type;
	int		fd;
	int		rint;
	char	*path;

	mov = dir;
	while (mov && !(*ex)->error_flag)
	{
		type = ((t_pack *)mov->content)->type;
		rint = -1;
		if (type == RINT)
		{
			rint = rint_atoi(((t_pack *)mov->content)->line);
			if (rint == -1 || rint > 255)
			{
				rint_error(((t_pack *)mov->content)->line, rint, m_sh);
				(*ex)->error_flag = true;
				rint = -2;
			}
			mov = mov->next;
		}
		type = ((t_pack *)mov->content)->type;
		mov = mov->next;
		if (!(path = path_make(((t_pack *)mov->content)->line, m_sh)))
			if (ambiguous_error(m_sh, ((t_pack *)mov->content)->line, ex))
				break ;
		fd_get(m_sh, path, type, rint);
		if (errno && ((*ex)->error_flag = true))
		{
			fd_error(path, 2);
			m_sh->exit_status = 1;
		}
//		solve_rint(fd, rint, ex, m_sh);
//		fd_reget(fd, rint, ex, type);
		free(path);
		mov = mov->next;
	}
}
