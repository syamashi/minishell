/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 14:45:21 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/13 13:25:24 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

void	sh_launch_child(
	t_minishell *m_sh, t_list *exlist, int *pipefd, int prev_pipe)
{
	t_builtin_f	builtin_function;
	t_exec		*exec_param;
	int			errno_reserve;

	signal(SIGINT, SIG_DFL);
	exec_param = exlist->content;
	if (prev_pipe)
		sh_dup_close(prev_pipe, 0);
	if (exec_param->fd_in != 0)
		sh_dup_close(exec_param->fd_in, 0);
	if (exlist->next)
		sh_dup_close(pipefd[1], 1);
	if (exec_param->fd_out != 1)
		sh_dup_close(exec_param->fd_out, 1);
	if (exec_param->error_flag)
		exit(1);
	if ((builtin_function = builtin_table(exec_param)))
	{
		exec_param->fd_in = 0;
		exec_param->fd_out = 1;
		exit(builtin_function(m_sh, exec_param));
	}
	sh_execvpes(exec_param, m_sh);
	printf("%d\n", errno);
	errno_reserve = errno;
	ft_perror("minishell");
	exit(errno_reserve);
}

int		sh_process_manager(t_minishell *m_sh, t_list *execlist, int prev_pipe)
{
	pid_t	cpid;
	int		status;
	int		pipefd[2];

	status = 0;
	if (execlist->next && pipe(pipefd) < 0)
		return (ft_perror("minishell"));
	if ((cpid = fork()) < 0)
		return (ft_perror("minishell"));
	if (cpid == 0)
		sh_launch_child(m_sh, execlist, pipefd, prev_pipe);
	if (!execlist->next && waitpid(cpid, &status, 0) < 0)
		return (ft_perror("minishell"));
	m_sh->exit_status = WEXITSTATUS(status);
	if (prev_pipe && close(prev_pipe) < 0)
		return (ft_perror("minishell"));
	if (execlist->next && close(pipefd[1]) < 0)
		return (ft_perror("minishell"));
	if (execlist->next)
		sh_process_manager(m_sh, execlist->next, pipefd[0]);
	return (0);
}

int		sh_launch(t_minishell *m_sh, t_list *execlist)
{
	t_builtin_f	builtin_function;

	if (!execlist->next && (builtin_function = builtin_table(execlist->content)))
	{
		if (((t_exec *)execlist->content)->error_flag)
			return ((m_sh->exit_status = -1));
		m_sh->exit_status = builtin_function(m_sh, execlist->content);
		return (m_sh->exit_status);
	}
	signal(SIGINT, SIG_IGN);
	sh_process_manager(m_sh, execlist, 0);
	signal(SIGINT, SIG_DFL);
	return (m_sh->exit_status);
}
