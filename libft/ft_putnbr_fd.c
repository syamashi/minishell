/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:18:25 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/25 01:15:03 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	m;

	m = n;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		m = -m;
	}
	if (m <= 9)
	{
		ft_putchar_fd(m + '0', fd);
		return ;
	}
	ft_putnbr_fd(m / 10, fd);
	ft_putchar_fd(m % 10 + '0', fd);
}
