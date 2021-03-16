/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 14:45:21 by ewatanab          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/03/16 09:44:16 by syamashi         ###   ########.fr       */
=======
/*   Updated: 2021/03/16 08:43:58 by syamashi         ###   ########.fr       */
>>>>>>> 5ae78766f28bb7ae6c8848f981fb5d3f747f0ab5
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
	return (126);
}

void	sh_launch_child(
	t_minishell *m_sh, t_list *exlist, int *pipefd, int prev_pipe)
{
	t_builtin_f	builtin_function;
	t_exec		*exec_param;

	exec_param = exlist->content;
	if (prev_pipe)
		sh_dup_close(prev_pipe, 0);
	if (exec_param->fd_in != 0)
		sh_dup_close(exec_param->fd_in, 0);
	if (exlist->next)
		sh_dup_close(pipefd[1], 1);
	if (exec_param->fd_out != 1)
		sh_dup_close(exec_param->fd_out, 1);
	if (exec_param->fd_err != 2)
		sh_dup_close(exec_param->fd_err, 2);
	if (exec_param->error_flag)
		exit(1);
	if ((builtin_function = builtin_table(exec_param)))
	{
		exec_param->fd_in = 0;
		exec_param->fd_out = 1;
		exec_param->fd_err = 2;
		exit(builtin_function(m_sh, exec_param));
	}
	sh_execvpes(exec_param, m_sh);
<<<<<<< HEAD
	ft_perror("minishell");
/*    if (errno == ENOENT)
    {
=======
	if (errno == ENOENT)
	{
>>>>>>> 5ae78766f28bb7ae6c8848f981fb5d3f747f0ab5
		ft_putstr_fd(MINISHELL, exec_param->fd_err);
		ft_putstr_fd(exec_param->argv[0], exec_param->fd_err);
		ft_putstr_fd(": ", exec_param->fd_err);
		ft_putstr_fd("command not found\n", exec_param->fd_err);
<<<<<<< HEAD
    }
	*/	
=======
	}
	else
		ft_perror("minishell");
>>>>>>> 5ae78766f28bb7ae6c8848f981fb5d3f747f0ab5
	exit(status_handling(errno));
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
	if (WIFSIGNALED(status)) // signal終了の判定
		m_sh->exit_status = WTERMSIG(status) + 128; //signalがとれる
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
		printf("[builtin]\n");
		if (((t_exec *)execlist->content)->error_flag)
			return ((m_sh->exit_status));
		return(m_sh->exit_status = builtin_function(m_sh, execlist->content));
	}
//	signal(SIGINT, SIG_IGN);
	if (signal(SIGINT, sh_putendl_handler) == SIG_ERR)
		exit(ft_error("sigerror", 1, STDERR));
	if (signal(SIGQUIT, sh_quithandler) == SIG_ERR)
		exit(ft_error("sigerror", 1, STDERR));
	sh_process_manager(m_sh, execlist, 0);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (m_sh->exit_status);
}