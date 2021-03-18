/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 14:45:21 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/18 13:33:15 by ewatanab         ###   ########.fr       */
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

int		usage_dot(int ret, int fd_err)
{
	ft_putstr_fd(MINISHELL, fd_err);
	ft_putstr_fd(".: filename argument required\n", fd_err);
	ft_putstr_fd(".: usage: . filename [arguments]\n", fd_err);
	return (ret);
}

void	launch_builtin(t_minishell *m_sh, t_exec *exec_param,
		t_builtin_f builtin_function)
{
	exec_param->fd_in = 0;
	exec_param->fd_out = 1;
	exec_param->fd_err = 2;
	exit(builtin_function(m_sh, exec_param));
}

void	sh_launch_child(
	t_minishell *m_sh, t_list *exlist, int *pipefd, int prev_pipe)
{
	t_builtin_f		builtin_function;
	t_exec			*exec_param;

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
	if (exlist->next)
		close(pipefd[0]);
	if (exec_param->error_flag)
		exit(1);
	if ((builtin_function = builtin_table(exec_param)))
		launch_builtin(m_sh, exec_param, builtin_function);
	if (!exec_param->argv[0])
		exit(0);
	if (!ft_strncmp(exec_param->argv[0], ".", 2))
		exit(usage_dot(2, STDERR));
	exec_command(m_sh, exec_param);
}

int		sh_process_manager(t_minishell *m_sh, t_list *execlist, int prev_pipe)
{
	pid_t	cpid;
	int		status;
	int		pipefd[2];

	status = 0;
	if (execlist->next && pipe(pipefd) < 0)
		return (ft_perror("", STDERR));
	if ((cpid = fork()) < 0)
		return (ft_perror("", STDERR));
	if (cpid == 0)
		sh_launch_child(m_sh, execlist, pipefd, prev_pipe);
	if (prev_pipe && close(prev_pipe) < 0)
		return (ft_perror("", STDERR));
	if (execlist->next && close(pipefd[1]) < 0)
		return (ft_perror("", STDERR));
	if (execlist->next)
		sh_process_manager(m_sh, execlist->next, pipefd[0]);
	if (waitpid(cpid, &status, 0) < 0)
		return (ft_perror("", STDERR));
	if (!execlist->next)
		m_sh->exit_status = WEXITSTATUS(status);
	if (!execlist->next && WIFSIGNALED(status))
		return (m_sh->exit_status = WTERMSIG(status) + 128);
	return (0);
}

int		sh_launch(t_minishell *m_sh, t_list *exlist)
{
	t_builtin_f	builtin_function;

	if (!exlist->next && (builtin_function = builtin_table(exlist->content)))
	{
		if (((t_exec *)exlist->content)->error_flag)
			return ((m_sh->exit_status));
		return (m_sh->exit_status = builtin_function(m_sh, exlist->content));
	}
	if (signal(SIGINT, sh_putendl_handler) == SIG_ERR)
		exit(ft_error("sigerror", 1, STDERR));
	if (signal(SIGQUIT, sh_quithandler) == SIG_ERR)
		exit(ft_error("sigerror", 1, STDERR));
	sh_process_manager(m_sh, exlist, 0);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (m_sh->exit_status);
}
