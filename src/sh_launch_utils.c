/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 15:02:59 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/12 13:05:18 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

int		sh_execvpes(t_exec *s)
{
	return (ft_execvpe(s->argv[0], s->argv, s->envp));
	//execvp(s->argv[0], s->argv);
}

int		ft_perror(char *string)
{
	int	a_errno = errno;

	ft_putstr_fd(string, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(a_errno), 2);
	ft_putstr_fd("\n", 2);
	return (-1);
}

void	sh_dup_close(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) < 0)
	{
		ft_perror("minishell");
		exit(1);
	}
	if (close(old_fd) < 0)
	{
		ft_perror("minishell");
		exit(1);
	}
}

t_builtin_f	builtin_table(t_exec *com)
{
	if (!ft_strcmp(com->argv[0], "echo"))
		return (sh_echo);
	return (NULL);
}
