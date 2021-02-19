/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:19:02 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/24 22:34:58 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		ssz;
	char	*str;

	ssz = ft_strlen(s);
	str = (char *)s;
	while (ssz >= 0)
	{
		if (str[ssz--] == c)
			return (str + ssz + 1);
	}
	return (NULL);
}
