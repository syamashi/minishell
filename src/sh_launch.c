/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 14:45:21 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/17 15:13:53 by ewatanab         ###   ########.fr       */
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
	ft_putstr_fd(MINISHELL, STDERR);
	ft_putstr_fd(".: filename argument required\n", STDERR);
	ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR);
	return (ret);
}

void	execvp_error(char *path, char *str, int ret)
{
	ft_putstr_fd(MINISHELL, STDERR);
	ft_putstr_fd(path, STDERR);
	ft_putstr_fd(": ", STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putstr_fd("\n", STDERR);
	exit(ret);
}

void	sh_launch_child(
	t_minishell *m_sh, t_list *exlist, int *pipefd, int prev_pipe)
{
	t_builtin_f		builtin_function;
	t_exec			*exec_param;
	struct stat		sb;
	int				errno_recieve;

	exec_param = exlist->content;

	if (prev_pipe)
		sh_dup_close(prev_pipe, 0, exec_param->fd_err); // prec0dup created, prec0, close
	if (exec_param->fd_in != 0)
		sh_dup_close(exec_param->fd_in, 0, exec_param->fd_err); // fdin
	if (exlist->next)
		sh_dup_close(pipefd[1], 1, exec_param->fd_err);
	if (exec_param->fd_out != 1)
		sh_dup_close(exec_param->fd_out, 1, exec_param->fd_err);
	if (exec_param->fd_err != 2)
		sh_dup_close(exec_param->fd_err, 2, exec_param->fd_err);


	if (exlist->next)
		close(pipefd[0]); //***
	if (exec_param->error_flag)
		exit(1);
	if ((builtin_function = builtin_table(exec_param)))
	{
		exec_param->fd_in = 0;
		exec_param->fd_out = 1;
		exec_param->fd_err = 2;
		exit(builtin_function(m_sh, exec_param));
	}
	// NULLのとき落ちる
	if (!exec_param->argv[0])
		exit(0);
	if (!ft_strncmp(exec_param->argv[0], ".", 2))
		exit(usage_dot(2, STDERR));
	if (!(*exec_param->argv[0]) || sh_execvpes(exec_param, m_sh) == -2)
		execvp_error(exec_param->argv[0], "command not found", 127);
	else if (errno)
	{
		errno_recieve = errno;
		if (stat(exec_param->argv[0], &sb) == 0)
		{
			if (!(sb.st_mode & S_IRUSR) || !(sb.st_mode & S_IXUSR))
				execvp_error(exec_param->argv[0], "Permission denied", 126);
			else
				execvp_error(exec_param->argv[0], "is a directory", 126);
		}
		errno = errno_recieve;
		ft_perror(exec_param->argv[0], STDERR);
	}
	close(0);
	close(1);
	close(2);
	exit(status_handling(errno));
}

int		sh_process_manager(t_minishell *m_sh, t_list *execlist, int prev_pipe)
{
	pid_t	cpid;
	int		status;
	int		pipefd[2];

	status = 0;
	if (execlist->next && pipe(pipefd) < 0)  // fd + 2 = 3
		return (ft_perror("", STDERR));
	if ((cpid = fork()) < 0) // fd * 2 = 4 {p0, p1, c0, c1, prep0, prec0}
		return (ft_perror("", STDERR));
	if (cpid == 0)
		sh_launch_child(m_sh, execlist, pipefd, prev_pipe);


	if (prev_pipe && close(prev_pipe) < 0) //prep0 close
		return (ft_perror("", STDERR));
	if (execlist->next && close(pipefd[1]) < 0) //p1 close
		return (ft_perror("", STDERR));

	if (execlist->next)
		sh_process_manager(m_sh, execlist->next, pipefd[0]); //p0 to be close

	if (waitpid(-1, &status, 0) < 0)
		return (ft_perror("", STDERR));
	m_sh->exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status)) // signal終了の判定
		return (m_sh->exit_status = WTERMSIG(status) + 128); //signalがとれる
	return (0);
}

int		sh_launch(t_minishell *m_sh, t_list *execlist)
{
	t_builtin_f	builtin_function;

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
	sh_process_manager(m_sh, execlist, 0);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (m_sh->exit_status);
}
