/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:18:49 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/26 16:53:31 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t ssz;
	size_t cnt;

	if (!dst)
		return (0);
	ssz = ft_strlen(src);
	if (dstsize == 0)
		return (ssz);
	cnt = dstsize - 1;
	while (cnt-- && *src)
		*dst++ = *src++;
	*dst = '\0';
	return (ssz);
}
