/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init2_shlvl.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 21:25:27 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/18 17:46:54 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"
#include "../includes/sh_launch.h"

int			shlvl_atoi(const char *nptr)
{
	char		*str;
	long long	n;
	long long 	num;
	long long	m;

	str = (char *)nptr;
	while (*str == ' ' || *str == '\t' ||
	*str == '\n' || *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	n = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-')
		str++;
	m = 0;
	while (ft_isdigit(*str))
	{
		num = (*str++ - '0');
		if ((m >= 922337203685477580 && num > 7) || m >= 922337203685477581)
			return (0);
		if ((m <= -922337203685477580 && num > 8) || m <= -922337203685477581)
			return (0);
		m = m * 10 + n * num;
	}
	while (is_space(*str))
		str++;
	return (*str ? 0 : m);
}

void		env_shlvl_init(t_minishell *m_sh)
{
	char	*key;
	char	*value;
	int		depth;

	if (!(key = ft_strdup("SHLVL")))
		exit(ft_error("malloc failed", 1, STDERR));
	if (!(value = value_get(key, m_sh)))
		depth = 0;
	else
		depth = shlvl_atoi(value);
	free(value);
	depth++;
	if (depth > 1000)
		shlvl_error(depth);
	depth = (depth < 0) ? 0 : depth;
	depth = (depth > 1000) ? 1 : depth;
	if (depth == 1000)
		value = ft_strdup("");
	else
		value = ft_itoa(depth);
	if (!value)
		exit(ft_error("malloc failed", 1, STDERR));
	export_envp(m_sh, key, value);
}
