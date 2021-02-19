/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 22:15:35 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/28 19:39:50 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
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
	while (ft_isdigit(*str))
	{
		num = (*str++ - '0');
		if ((m >= 922337203685477580 && num > 7) || m >= 922337203685477581)
			return (-1);
		if ((m <= -922337203685477580 && num > 8) || m <= -922337203685477581)
			return (0);
		m = m * 10 + n * num;
	}
	return (m);
}
