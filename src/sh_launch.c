/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 14:45:21 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/16 15:09:29 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

/*
**  bash-3.2$ /bin/asdf/asdf ; echo $?
**  bash: /bin/asdf/asdf: No such file or directory
**  127
**
**  bash-3.2$ /bin/ls/asdf ; echo $?
**  bash: /bin/ls/asdf: Not a directory
**  126
**
**  ENOENT 2 No such file or directory
**  ENOTDIR 20 Not a directory
*/

int		status_handling(int e)
{
	if (e == ENOENT)
		return (127);
//	else if (errno == 20 || errno == 13)
//		return (126);
	else
		return (126);
}

int		usage_dot(int ret, int fd_err)
{
	ft_putstr_fd(MINISHELL, fd_err);
	ft_putstr_fd(".: filename argument required\n", fd_err);
	ft_putstr_fd(".: usage: . filename [arguments]\n", fd_err);
	return (ret);
}

void	sh_launch_child(
	t_minishell *m_sh, t_list *exlist, int *pipefd, int prev_pipe)
{
	t_builtin_f		builtin_function;
	t_exec			*exec_param;
	struct	stat	sb;

	exec_param = exlist->content;
	if (prev_pipe)
		sh_dup_close(prev_pipe, 0, exec_param->fd_err);
	if (exec_param->fd_in != 0)
		sh_dup_close(exec_param->fd_in, 0, exec_param->fd_err);
	if (exlist->next)
		sh_dup_close(pipefd[1], 1, exec_param->fd_err);
	if (exec_param->fd_out != 1)
		sh_dup_close(exec_param->fd_out, 1, exec_param->fd_err);
	if (exec_param->fd_err != 2)
		sh_dup_close(exec_param->fd_err, 2, exec_param->fd_err);
	if (exec_param->error_flag)
		exit(1);
	if ((builtin_function = builtin_table(exec_param)))
	{
		exec_param->fd_in = 0;
		exec_param->fd_out = 1;
		exec_param->fd_err = 2;
		exit(builtin_function(m_sh, exec_param));
	}
	if (!exec_param->argv[0])
		exit(0);
	if (!ft_strncmp(exec_param->argv[0], ".", 2))
		exit(usage_dot(2, exec_param->fd_err));
	sh_execvpes(exec_param, m_sh);
	if (errno)
	{
		int e = errno;
		int st = stat(exec_param->argv[0], &sb);
		printf("st:%d\n", st);
		if (errno)
			ft_perror(exec_param->argv[0], exec_param->fd_err);
		errno = e;
		ft_perror(exec_param->argv[0], exec_param->fd_err);
	}
/*    if (errno == ENOENT)
    {
		ft_putstr_fd(MINISHELL, exec_param->fd_err);
		ft_putstr_fd(exec_param->argv[0], exec_param->fd_err);
		ft_putstr_fd(": ", exec_param->fd_err);
		ft_putstr_fd("command not found\n", exec_param->fd_err);
    }
	*/	
	exit(status_handling(errno));
}

int		sh_process_manager(t_minishell *m_sh, t_list *execlist, int prev_pipe)
{
	pid_t	cpid;
	int		status;
	int		pipefd[2];

	status = 0;
	if (execlist->next && pipe(pipefd) < 0)
		return (ft_perror("", ((t_exec *)execlist->content)->fd_err));
	if ((cpid = fork()) < 0)
		return (ft_perror("", ((t_exec *)execlist->content)->fd_err));
	if (cpid == 0)
		sh_launch_child(m_sh, execlist, pipefd, prev_pipe);
//	if (!execlist->next && waitpid(cpid, &status, 0) < 0)
//		return (ft_perror("", ((t_exec *)execlist->content)->fd_err));
	if (WIFSIGNALED(status)) // signal終了の判定
		return (-1); //signalがとれる
	if (prev_pipe && close(prev_pipe) < 0)
		return (ft_perror("", ((t_exec *)execlist->content)->fd_err));
	if (execlist->next && close(pipefd[1]) < 0)
		return (ft_perror("", ((t_exec *)execlist->content)->fd_err));
	if (execlist->next)
		return (sh_process_manager(m_sh, execlist->next, pipefd[0]));
	return (cpid);
}

int		sh_launch(t_minishell *m_sh, t_list *execlist)
{
	t_builtin_f	builtin_function;
	int			pipe_cnt;
	int			status;
	int			last_pid;

	if (!execlist->next && (builtin_function = builtin_table(execlist->content)))
	{
		if (((t_exec *)execlist->content)->error_flag)
			return ((m_sh->exit_status));
		return(m_sh->exit_status = builtin_function(m_sh, execlist->content));
	}
//	signal(SIGINT, SIG_IGN);
	if (signal(SIGINT, sh_putendl_handler) == SIG_ERR)
		exit(ft_error("sigerror", 1, STDERR));
	if (signal(SIGQUIT, sh_quithandler) == SIG_ERR)
		exit(ft_error("sigerror", 1, STDERR));
	last_pid = sh_process_manager(m_sh, execlist, 0);
	pipe_cnt = -1;
	while(++pipe_cnt < ft_lstsize(execlist))
	{
		if (wait(&status) == last_pid)
			m_sh->exit_status = status;
		if (status == 2 || status == 3)
			m_sh->exit_status += 128;
	}
	
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (m_sh->exit_status);
}
