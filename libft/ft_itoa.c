/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:23:06 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/25 22:08:27 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	intlen(long n)
{
	int len;

	len = 0;
	if (n < 0)
	{
		n = -n;
		len++;
	}
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char		*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	m;
	int		i;

	len = (n == 0) ? 1 : intlen(n);
	if (!(str = (char*)malloc(len + 1)))
		return (NULL);
	m = n;
	if (n < 0)
	{
		str[0] = '-';
		m = -m;
	}
	if (n == 0)
		str[0] = '0';
	i = len;
	while (m)
	{
		str[--i] = m % 10 + '0';
		m /= 10;
	}
	str[len] = '\0';
	return (str);
}
