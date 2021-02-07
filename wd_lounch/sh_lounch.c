/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_lounch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 14:45:21 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/07 19:40:24 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sh_lounch.h"

void	sh_lounch_child(t_exec *exec_param, int *pipefd, int prev_pipe, bool has_next)
{
	t_builtin_f	builtin_function;

	if (prev_pipe)
		sh_dup_close(prev_pipe, 0);
	if (exec_param->fd_in != 0)
		sh_dup_close(exec_param->fd_in, 0);
	if (has_next)
		sh_dup_close(pipefd[1], 1);
	if (exec_param->fd_out != 1)
		sh_dup_close(exec_param->fd_out, 1);
	if ((builtin_function = builtin_table(exec_param)))
		exit(builtin_function(exec_param));
	sh_execvpes(exec_param);
	ft_perror("minishell");
	exit(1);
}

int		sh_process_manager(t_list *execlist, int prev_pipe)
{
	pid_t	cpid;
	int		status;
	int		pipefd[2];
	t_exec	*exec_param;

	exec_param = execlist->content;
	if (execlist->next && pipe(pipefd) < 0)
		return (ft_perror("minishell"));
	if ((cpid = fork()) < 0)
		return (ft_perror("minishell"));
	if (cpid == 0)
		sh_lounch_child(exec_param, pipefd, prev_pipe, (execlist->next != NULL));
	if (waitpid(cpid, &status, 0) < 0)
		return (ft_perror("minishell"));
	if (prev_pipe && close(prev_pipe) < 0)
		return (ft_perror("minishell"));
	if (execlist->next && close(pipefd[1]) < 0)
		return (ft_perror("minishell"));
	if (execlist->next)
		sh_process_manager(execlist->next, pipefd[0]);
	return (0);
}

int		sh_lounch(t_list *execlist)
{
	t_builtin_f	builtin_function;

	if ((builtin_function = builtin_table(execlist->content)))
		return(builtin_function(execlist->content));
	sh_process_manager(execlist, 0);
	return (0);
}

