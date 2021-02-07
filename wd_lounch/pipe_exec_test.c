# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include <errno.h>
# include <stdbool.h>
# include <fcntl.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>

int		reading_test();

int		main()
{
	int pipefd[2];
	int status;
	pid_t	pid;
	char *ls[] = {"ls", NULL};
	char *grep[] = {"grep", "pipe", NULL};
	//char *cat[] = {"cat", NULL};

	if (pipe(pipefd) < 0)
		return (1);

	pid = fork();
	if (pid == 0)
	{
		dup2(pipefd[1], 1);
		close(pipefd[1]);
	//	write(1, "test", 4);
		execvp(ls[0], ls);
		exit(0);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
//	write(pipefd[1], "\255", 1);

	pid = fork();
	if (pid == 0)
	{
		dup2(pipefd[0], 0);
		close(pipefd[0]);
	//	reading_test();
		execvp(grep[0], grep);
		exit(0);
	}
	close(pipefd[0]);
	waitpid(pid, &status, 0);

	return (0);
}
