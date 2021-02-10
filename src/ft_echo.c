/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/09 16:28:09 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/08 15:33:31 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

bool	echo_nflag_parse(char *arg)
{
	if (*argv++ != '-')
		return (false);
	while (*argv++ == 'n')
		;
	if (*argv)
		return (false);
	return (true);
}

int		ft_echo(t_exec *com)
{
	char	**argv;
	bool	opt_n;

	argv = com->argv;
	if (!argv)
		return (-1);
	opt_n = false;
	while ((opt_n |= echo_nflag_parse(*argv)))
		argv++;
	while (*argv)
	{
		ft_putstr_fd((*argv)++, com->fd_out);
		if (*argv)
			ft_putchar_fd(' ', com->fd_out);
	}
	if (!opt_n)
		ft_putchar_fd('\n', com->fd_out);
	return (0);
}

int		ft_cd(char **argv)
{
	if (!argv)
		return (-1);
	if (argv[2])
		return (-1);
	return (chdir(argv[1]));
}
