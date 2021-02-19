/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:18:20 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/26 21:47:51 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define X1 0x01
#define X2 0x02
#define X3 0x04
#define X4 0x08
#define X5 0x10
#define X6 0x20
#define X7 0x40
#define X8 0x80
#define FC1 0x80
#define FC2 0x800

void	ft_putchar_fd(char c, int fd)
{
	unsigned char uc;
	unsigned char chr;

	uc = (unsigned char)c;
	chr = uc;
	if (chr < FC1)
	{
		write(fd, &chr, 1);
		return ;
	}
	else
	{
		chr = X8 + X7 + uc / X7;
		write(fd, &chr, 1);
		chr = X8 + uc % X7;
		write(fd, &chr, 1);
		return ;
	}
}
