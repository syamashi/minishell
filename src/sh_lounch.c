/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_lounch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 14:45:21 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/01 15:39:49 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sh_execvpes(t_exec *s)
{
	ft_execvpe(s->argv[0], s->argv, s->envp);
}

void	ft_perror(char *string)
{
	int	a_errno = errno;

	ft_putstr_fd(string, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(a_errno), 2);
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


int		sh_lounch_process(t_list *execlist, int prev_pipe)
{
	pid_t	cpid;
	int		status;
	int		pipefd[2];
	t_exec	*exec_param;

	exec_param = execlist->content;
	if (execlist->next)
	{
		if (pipe(pipefd) < 0)
		{
			strerror("minishell");
			return (-1);
		}
	}
	cpid = fork();
	if (cpid == 0)
	{
		if (prev_pipe)
			sh_dup_close(prev_pipe, 0);
		if (((t_exec *)(execlist->content))->fd_in != 0)
		{
			dup2(((t_exec *)(execlist->content))->fd_in, 0);
			close(((t_exec *)(execlist->content))->fd_in);
		}
		if (ft_lstsize(execlist) > 1)
		{
			dup2(pipefd[1], 1);
			close(pipefd[1]);
		}
		if (((t_exec *)(execlist->content))->fd_in != 1)
		{
			dup2(((t_exec *)(execlist->content))->fd_out, 1);
			close(((t_exec *)(execlist->content))->fd_out);
		}
		sh_execvpes(execlist->content);
		perror("minishell");
		exit(1);
	}
	waitpid(cpid, &status, 0);
	if (ft_lstsize(execlist) > 1)
		sh_lounch_process(execlist->next, pipefd[0]);
	return (0);
}

int		sh_lounch(t_list *execlist)
{
	/*
	 * if (check_builtin(execlist) && ft_lstsize(execlist) == 1)
	 *	exec_builtin(execlist->content);
	 */


	return (0);

}

