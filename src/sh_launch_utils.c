/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 15:02:59 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/18 15:49:11 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

bool		if_executable(int mode)
{
	if (!(mode & S_IRUSR))
		return (false);
	if ((mode & S_IRWXU) == S_IRWXU)
		return (true);
	if ((mode & S_IRWXG) == S_IRWXG)
		return (true);
	if ((mode & S_IRWXO) == S_IRWXO)
		return (true);
	return (false);
}

void		exec_command(t_minishell *m_sh, t_exec *exec_param)
{
	char	*env_path;

	env_path = value_get("PATH", m_sh);
	simple_case(exec_param->argv[0], exec_param->argv, exec_param->envp);
	search_and_exec(env_path, exec_param->argv, exec_param->envp);
	free(env_path);
}

int			ft_perror(char *string, int fd_err)
{
	int		errno_reserve;

	errno_reserve = errno;
	ft_putstr_fd(MINISHELL, fd_err);
	ft_putstr_fd(string, fd_err);
	ft_putstr_fd(": ", fd_err);
	ft_putstr_fd(strerror(errno_reserve), fd_err);
	ft_putstr_fd("\n", fd_err);
	return (-1);
}

void		sh_dup_close(int old_fd, int new_fd, int fd_err)
{
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
