/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_lounch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 13:32:17 by ewatanab          #+#    #+#             */
/*   Updated: 2021/01/30 14:34:18 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_list_size(t_commands *lists)
{
	int		tmp_size;

	tmp_size = ft_lstsize(lists->com);
	if (ft_lstsize(lists->args) != tmp_size)
		return (-1);
	if (ft_lstsize(lists->input) != tmp_size)
		return (-1);
	if (ft_lstsize(lists->output) != tmp_size)
		return (-1);
	return (0);
}

static	int (*builtin_search(char *command))()
{
	if (!ft_strcmp(command, "echo"))
		return (sh_echo);
	if (!ft_strcmp(command, "cd"))
		return (sh_cd);
	if (!ft_strcmp(command, "env"))
		return (sh_env);
	if (!ft_strcmp(command, "export"))
		return (sh_export);
	if (!ft_strcmp(command, "unset"))
		return (sh_unset);
	if (!ft_strcmp(command, "exit"))
		return (sh_exit);
	return (NULL);
}

static void	rec(t_commands *commands)
{
	pid_t	pid;
	int		status;
	int		(*builtin)();

	dup2((intptr_t)commands->input->content, STDIN_FILENO);
	dup2((intptr_t)commands->input->content, STDOUT_FILENO);
	if (commands->com->next)
	{
		commands_pop(commands);
		pid = fork();
		if (pid)
			rec(commands);
		wait(&status);
		if ((intptr_t)commands->input->content != STDIN_FILENO)
			close((intptr_t)commands->input->content);
		if ((intptr_t)commands->output->content != STDOUT_FILENO)
			close((intptr_t)commands->output->content);
		commands_clear(commands);
	}
	if ((builtin = builtin_search(commands->com->content)))
		sh_exit(builtin);
	ft_execvpe(argv[0], argv, envp);
	//error message
	sh_exit(-1);
}

int			sh_lounch(t_commands *commands)
{
	pid_t	pid;
	int		status;
	int		(*builtin)();

	if (check_list_size(commands) < 0)
		return (-1);
	if (!commands->com->next && (builtin = builtin_search(commands->com->content)))
		return (builtin(commands));
	pid = fork();
	if (pid)
		rec(commands);
	wait(&status);
	if ((intptr_t)commands->input->content != STDIN_FILENO)
		close((intptr_t)commands->input->content);
	if ((intptr_t)commands->output->content != STDOUT_FILENO)
		close((intptr_t)commands->output->content);
	commands_clear(commands);
	return (WEXITSTATUS(status));
}
