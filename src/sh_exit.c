/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:28:47 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/17 19:50:04 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

static	int	get_exitnum(const char *nptr)
{
	char		*str;
	int			n;
	int			num;
	long long	m;

	str = (char *)nptr;
	while (*str == ' ' || *str == '\t' ||
	*str == '\n' || *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	n = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-')
		str++;
	m = 0;
	if (!*str)
		return (4242);
	while (ft_isdigit(*str))
	{
		num = (*str++ - '0');
		if ((m >= 922337203685477580 && num > 7) || m >= 922337203685477581)
			return (4242);
		if ((m <= -922337203685477580 && num > 8) || m <= -922337203685477581)
			return (4242);
		m = m * 10 + n * num;
	}
	return (*str ? 4242 : ((m % 256) + 256) % 256);
}

int			sh_exit(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;
	int		ret;

	argv = exec->argv + 1;
	ft_putstr_fd("exit\n", exec->fd_err);
	if (!*argv)
		exit(m_sh->exit_status);
	if ((ret = get_exitnum(*argv)) > 255)
		exit(ft_exit_error(*argv, 255, exec->fd_err));
	if (*(argv + 1))
		return (ft_error("exit: too many arguments", 1, exec->fd_err));
	exit(ret);
}
