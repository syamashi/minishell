/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 15:02:59 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/16 17:36:56 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

int			sh_execvpes(t_exec *s, t_minishell *m_sh)
{
	return (sh_execvpe(s->argv[0], s->argv, s->envp, m_sh));
	//execvp(s->argv[0], s->argv);
}

int			ft_perror(char *string, int fd_err)
{
	int	a_errno = errno;

	ft_putstr_fd(MINISHELL, fd_err);
	ft_putstr_fd(string, fd_err);
	ft_putstr_fd(": ", fd_err);
	ft_putstr_fd(strerror(a_errno), fd_err);
	ft_putstr_fd("\n", fd_err);
	return (-1);
}

void		sh_dup_close(int old_fd, int new_fd, int fd_err)
{
//	printf("[sh_dup_close] old:%d, new:%d, fd_err:%d\n", old_fd, new_fd, fd_err);
	if (dup2(old_fd, new_fd) < 0)
	{
		ft_perror("", fd_err);
		exit(1);
	}
	if (close(old_fd) < 0)
	{
		ft_perror("", fd_err);
		exit(1);
	}
}

t_builtin_f	builtin_table(t_exec *com)
{
	if (!com->argv[0])
		return (NULL);
	if (!ft_strcmp(com->argv[0], "echo"))
		return (sh_echo);
	if (!ft_strcmp(com->argv[0], "export"))
		return (sh_export);
	if (!ft_strcmp(com->argv[0], "unset"))
		return (sh_unset);
	if (!ft_strcmp(com->argv[0], "env"))
		return (sh_env);
	if (!ft_strcmp(com->argv[0], "cd"))
		return (sh_cd);
	if (!ft_strcmp(com->argv[0], "pwd"))
		return (sh_pwd);
	if (!ft_strcmp(com->argv[0], "exit"))
		return (sh_exit);
	return (NULL);
}
