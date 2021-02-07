/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 13:22:04 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/06 13:59:00 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

int		reading_test();

int		main()
{
	int pipefd[2];
	int status;
	pid_t	pid;

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (1);
	}
	if ((pid = fork()) < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		write(1, "test", 4);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if ((pid = fork()) < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		reading_test();
		exit(0);
	}
	waitpid(pid, &status, 0);
	return (0);
}
