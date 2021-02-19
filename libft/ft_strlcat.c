/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:18:46 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/24 22:34:49 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t n;
	size_t m;

	n = ft_strlen(dst);
	m = ft_strlen(src);
	m = (dstsize < n) ? m + dstsize : m + n;
	if (dstsize == 0)
		return (m);
	while (*src && n + 1 < dstsize)
		dst[n++] = *src++;
	dst[n] = '\0';
	return (m);
}
