/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:18:59 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/25 14:52:37 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t len)
{
	if (len == 0)
		return (0);
	while (len--)
	{
		if (*s1 != *s2++)
			return ((unsigned char)*s1 - (unsigned char)*(s2 - 1));
		if (*s1++ == '\0')
			break ;
	}
	return (0);
}
