/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit2_fdcontrol.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 15:41:55 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/18 17:00:32 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

bool	ambiguous_error(t_minishell *m_sh, char *str, t_exec **ex)
{
	errno = 0;
	m_sh->exit_status = dir_error(str, 1, ex);
	(*ex)->error_flag = true;
	if ((*ex)->fd_in != 0)
		close((*ex)->fd_in);
	(*ex)->fd_in = 0;
	if ((*ex)->fd_out != 1)
		close((*ex)->fd_out);
	(*ex)->fd_out = 1;
	if ((*ex)->fd_err != 2)
		close((*ex)->fd_err);
	(*ex)->fd_err = 2;
	return (true);
}

int		rint_atoi(const char *nptr)
{
	char		*str;
	long long	n;
	long long	num;
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
	if (m > 2147483647)
		return (-1);
	return (m);
}

void	rint_error(char *rint, int rint_num, t_minishell *m_sh, int fd)
{
	m_sh->exit_status = 1;
	if (rint_num == -1)
	{
		ft_putstr_fd(MINISHELL, fd);
		ft_putstr_fd("file descriptor out of range", fd);
	}
	else if (rint_num > 255)
	{
		ft_putstr_fd(MINISHELL, fd);
		ft_putstr_fd(rint, fd);
	}
	else if (rint_num > 2)
	{
		ft_avoid_error("3 or more fd", 1, fd);
		return ;
	}
	ft_putstr_fd(": Bad file descriptor\n", fd);
}

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

int		get_rint(t_pack *pack, t_exec **ex, t_minishell *m_sh)
{
	long long	rint;

	if (pack->type != RINT)
		return (-1);
	rint = rint_atoi(pack->line);
	if (rint == -1 || rint > 2)
	{
		rint_error(pack->line, rint, m_sh, (*ex)->fd_err);
		(*ex)->error_flag = true;
		rint = -2;
	}
	return (rint);
}

void	fd_controller(t_exec **ex, t_list *dir, t_minishell *m_sh)
{
	t_list	*mov;
	t_pack	*pack_rd;
	int		rint;
	char	*path;

	mov = dir;
	while (mov && !(*ex)->error_flag)
	{
		rint = get_rint(mov->content, ex, m_sh);
		if (((t_pack *)mov->content)->type == RINT)
			mov = mov->next;
		pack_rd = (t_pack *)mov->content;
		mov = mov->next;
		if (!(path = path_make(((t_pack *)mov->content)->line, m_sh)))
			if (ambiguous_error(m_sh, ((t_pack *)mov->content)->line, ex))
				break ;
		fd_get(ex, path, pack_rd->type, rint);
		if (errno && (*ex)->error_flag)
		{
			fd_error(path, (*ex)->fd_err);
			m_sh->exit_status = 1;
		}
		free(path);
		mov = mov->next;
	}
}
