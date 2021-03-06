/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putout_error2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:19:53 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/17 22:50:40 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

int		dir_error(char *path, int n, t_exec **ex)
{
	ft_putstr_fd(MINISHELL, (*ex)->fd_err);
	ft_putstr_fd(path, (*ex)->fd_err);
	ft_putstr_fd(": ambiguous redirect\n", (*ex)->fd_err);
	return (n);
}

int		ft_cd_free_error(char *path, int n, char *serror, int fd)
{
	ft_putstr_fd(MINISHELL, fd);
	ft_putstr_fd("cd: ", fd);
	if (path)
	{
		ft_putstr_fd(path, fd);
		ft_putstr_fd(": ", fd);
		free(path);
	}
	ft_putstr_fd(serror, fd);
	ft_putstr_fd("\n", fd);
	return (n);
}

int		cd_error(char *path, int n, char *serror, int fd)
{
	ft_putstr_fd(MINISHELL, fd);
	ft_putstr_fd("cd: ", fd);
	if (path)
	{
		ft_putstr_fd(path, fd);
		ft_putstr_fd(": ", fd);
	}
	ft_putstr_fd(serror, fd);
	ft_putstr_fd("\n", fd);
	return (n);
}

void	shlvl_error(int depth)
{
	char	*str;

	if (!(str = ft_itoa(depth)))
		exit(ft_error("malloc failed", 1, STDERR));
	ft_putstr_fd(MINISHELL, STDERR);
	ft_putstr_fd("warning: shell level (", STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putstr_fd(") too high, resetting to 1\n", STDERR);
	free(str);
}
