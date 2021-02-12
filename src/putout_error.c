/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putout_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:53:01 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/12 12:47:36 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

int ft_error(char *str, int	i)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);	
	return (i);
}

int	ft_syntax_error(char *str, int i)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd("\n", 2);
	return (i);
}

int	ft_avoid_error(char *str, int i)
{
	static int cnt;

	ft_putstr_fd("minishell: ", 2);
	if (cnt%3 == 0)
		ft_putstr_fd("（ﾟﾛﾟ）oops '", 2);
	if (cnt%3 == 1)
		ft_putstr_fd("(　ﾟдﾟ)oops '", 2);
	if (cnt%3 == 2)
		ft_putstr_fd("( Д)　゜゜oops '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(" is now developing...", 2);
	ft_putstr_ft("\n", 2);
	cnt++;
	return (i);
}

int	ft_exit_error(char *str, int i)
{
	ft_putstr_fd("bash: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": numeric argument required", 2);
	ft_putstr_fd("\n", 2);
	return (i);
}

void	fd_error(char *str, int fd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	errno = 0;
}
