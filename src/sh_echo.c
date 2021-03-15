/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 12:35:36 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/15 13:58:11 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

bool	if_opt_n(int flag)
{
	return (flag & F_ECHO_OPT_N);
}

/*
 * int parse_flag(char *str);
 * arg
 *  - str : string to parse
 * return value
 *  - 0 : if str is NOT flag
 *  - else : parsed flag bit
 */

int		parse_flag(char *str)
{
	int		flag;
	flag = 0;
	if (*str++ != '-')
		return (0);
	while (*str)
	{
		if (*str == 'n')
			flag |= F_ECHO_OPT_N;
		else
			return (0);
		str++;
	}
	return (flag);
}

int		sh_echo(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;
	int		flag;
	int		tmp;

	argv = exec->argv + 1;
	flag = 0;
	while (*argv && (tmp = parse_flag(*argv)))
	{
		flag |= tmp;
		argv++;
	}
	while (*argv)
	{
		ft_putstr_fd(*argv++, STDOUT);
		if (*argv)
			ft_putchar_fd(' ', STDOUT);
	}
	if (!if_opt_n(flag))
		ft_putchar_fd('\n', STDOUT);
	return (0);
}
