/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 13:18:30 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/06 14:07:38 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>

int		reading_test()
{
	char	buf[5];

	read(0, buf, 4);
	buf[4] = 0;
	if (strcmp(buf, "test"))
	{
		write(1, "NG\n", 3);
		return (-1);
	}
	else
	{
		write(1, "OK\n", 3);
		return (0);
	}
}

/*
 * int		main()
 * {
 * 	reading_test();
 * 	return (0);
 * }
 */
