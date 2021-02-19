/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:18:57 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/26 19:57:32 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*strb;
	char	*strl;
	int		n;
	size_t	m;

	strb = (char *)big;
	if (*little == '\0')
		return (strb);
	strl = (char *)little;
	n = len - ft_strlen(little) + 1;
	m = ft_strlen(little);
	while (1)
	{
		if (*strb == '\0' || n-- <= 0)
			return (NULL);
		if (ft_strncmp(strb++, strl, m) == 0)
			return (strb - 1);
	}
}
