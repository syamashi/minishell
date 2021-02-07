/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_lounch_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 15:02:59 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/06 20:32:03 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sh_lounch.h"

void	sh_execvpes(t_exec *s)
{
	//ft_execvpe(s->argv[0], s->argv, s->envp);
	execvp(s->argv[0], s->argv);
}

int		ft_perror(char *string)
{
	int	a_errno = errno;

	ft_putstr_fd(string, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(a_errno), 2);
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
