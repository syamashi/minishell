/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putout_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:53:01 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/16 10:40:00 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

int		ft_error(char *str, int i, int fd)
{
	ft_putstr_fd(MINISHELL, fd);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("\n", fd);
	return (i);
}

int		ft_syntax_error(char *str, int i)
{
	ft_putstr_fd(MINISHELL, STDERR);
	ft_putstr_fd("syntax error near unexpected token '", STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putstr_fd("'", STDERR);
	ft_putstr_fd("\n", STDERR);
	return (i);
}

int		ft_avoid_error(char *str, int i, int fd)
{
	static int cnt;

	ft_putstr_fd(MINISHELL, fd);
	if (cnt % 3 == 0)
		ft_putstr_fd("(*д*)oops '", fd);
	if (cnt % 3 == 1)
		ft_putstr_fd("(-д-)oops '", fd);
	if (cnt % 3 == 2)
		ft_putstr_fd("(;Д;)oops '", fd);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("'", fd);
	ft_putstr_fd(" does not support...", fd);
	ft_putstr_fd("\n", fd);
	cnt++;
	return (i);
}

int		ft_exit_error(char *str, int i)
{
	ft_putstr_fd(MINISHELL, STDERR);
	ft_putstr_fd("exit: ", STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putstr_fd(": numeric argument required", STDERR);
	ft_putstr_fd("\n", STDERR);
	return (i);
}

void	fd_error(char *str, int fd)
{
	ft_putstr_fd(MINISHELL, STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putstr_fd(": ", STDERR);
	ft_putstr_fd(strerror(errno), STDERR);
	ft_putstr_fd("\n", STDERR);
}
