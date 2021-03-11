/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit2_fdcontrol.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 15:41:55 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/21 19:22:52 by syamashi         ###   ########.fr       */
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
	if ((*ex)->fd_in != 0)
		close((*ex)->fd_in);
	(*ex)->fd_in = 0;
	if ((*ex)->fd_out != 1)
		close((*ex)->fd_out);
	(*ex)->fd_out = 1;
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

void	rint_error(char *rint, int rint_num)
{
	ft_putstr_fd("minishell: ", 2);
	if (rint_num == -1)
		ft_putstr_fd("file descriptor out of range", 2);
	if (rint_num > 255)
		ft_putstr_fd(rint, 2);
	ft_putstr_fd(": Bad file descriptor\n", 2);
}

int		fd_rdir(t_exec **ex, char *path, int rint_num)
{
	int	fd;
	
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	// 1>file か>fileだったら付け替え
	// それ以外だったら、とりあえずdup2()。復元していないので、pipeあとでも変わったまま
	if (rint_num == -1 || rint_num == 1)
	{
		if ((*ex)->fd_out != 1)
			close((*ex)->fd_out);
		(*ex)->fd_out = fd;
	}
	else
	{
		dup2(fd, rint_num);
		close(fd);
	}
	return (fd);
}

int		fd_rrdir(t_exec **ex, char *path, int rint_num)
{
	int	fd;
	
	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if ((*ex)->fd_out != 1)
		close((*ex)->fd_out);
	(*ex)->fd_out = fd;
	return (fd);
}

int		fd_ldir(t_exec **ex, char *path, int rint_num)
{
	int	fd;
	
	fd = open(path, O_RDONLY);
	if ((*ex)->fd_in != 0)
		close((*ex)->fd_in);
	(*ex)->fd_in = fd;
	return (fd);
}

int		fd_get(t_exec **ex, char *path, int type, char *rint)
{
	int fd;
	int rint_num;

	rint_num = (rint) ? rint_atoi(rint) : -1;
	errno = 0;
	if (type == RDIR)
		fd = fd_rdir(ex, path, rint_num);
	if (type == RRDIR)
		fd = fd_rrdir(ex, path, rint_num);
	if (type == LDIR)
		fd = fd_ldir(ex, path, rint_num);
	if (errno && ((*ex)->error_flag = true))
		fd_error(path, 2);
	return (fd);
}

void	solve_rint(int fd, char *rint, t_exec **ex, t_minishell *m_sh)
{
	int			rint_num;
	t_list		*new;
	t_redint	*rfd;

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
		exit(ft_error("", 1));
	rfd->rint = rint_num;
	rfd->backup = dup(rint_num);
	if (!(new = ft_lstnew(rfd)))
		exit(ft_error("", 1));
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

void	fd_controller(t_exec **ex, t_list *dir, t_minishell *m_sh)
{
	t_list	*mov;
	int		type;
	int		fd;
	char	*rint;
	char	*path;

	mov = dir;
	while (mov && !(*ex)->error_flag)
	{
		type = ((t_pack *)mov->content)->type;
		if (rint = (type == RINT) ? ((t_pack *)mov->content)->line : NULL)
			mov = mov->next;
		type = ((t_pack *)mov->content)->type;
		mov = mov->next;
		if (!(path = path_make(((t_pack *)mov->content)->line, m_sh)))
			if (ambiguous_error(m_sh, ((t_pack *)mov->content)->line, ex))
				break ;
		fd = fd_get(ex, path, type, rint);
//		solve_rint(fd, rint, ex, m_sh);
//		fd_reget(fd, rint, ex, type);
		free(path);
		mov = mov->next;
	}
}
