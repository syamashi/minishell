/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/09 16:28:09 by ewatanab          #+#    #+#             */
/*   Updated: 2020/09/09 17:16:08 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int		ft_echo(char **argv)
{
	bool	opt_n;

	if (!argv)
		return (-1);
	opt_n = false;
	if (*argv && !ft_strcmp(*argv, STR_OPT_LN))
		opt_n = true;
	while (*argv)
	{
		ft_putstr((*argv)++);
		if (*argv)
			ft_putchar(' ');
	}
	if (!opt_n)
		ft_putchar('\n');
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
