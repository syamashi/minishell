/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 15:26:32 by ewatanab          #+#    #+#             */
/*   Updated: 2021/01/30 14:34:26 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_exec	*sh_parse(char const *line)
{
	t_exec	*exec;
	char const	*iter;
	char *store;

	exec = calloc(1, sizeof(t_exec));
	iter = line;
	while (*iter)
	{
		if (is_space(*iter))
		{
			ft_lstadd_back(&exec->list, ft_lstnew(ft_substr(line, 0, iter - line)));
			while (is_space(*iter))
				iter++;
			line = iter;
			continue ;
		}
		if (*iter == '|')
		{
			t_exec *rhs = sh_parse(iter + 1);
			if (!rhs)
			{
				del_exec(exec);
				return (0);
			}
			int	pipefd[2];
			if (pipe(pipefd) < 0)
				exit(0);
			if (rhs->fd_in != STDIN_FILENO)
				rhs->fd_in = pipefd[0];
			if (exec->fd_in != STDOUT_FILENO)
				exec->fd_out = pipefd[1];
			exec->argv = ft_lsttov(exec->list);
			exec->envp = ft_lsttov(env_list); 
			pid_t	pid;
			pid = fork();
			if (pid == 0)
				return (exec);
			int	status;
			wait(&status);
			ft_execpve(rhs->argv[0], rhs->argv, rhs->envp);
		}
	}
	return (exec);
}
