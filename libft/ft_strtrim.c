/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:19:11 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/26 17:02:29 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		used[301];
	int		i;
	int		j;

	if (!s1)
		return (0);
	if (!set)
		return (ft_strdup(s1));
	i = 0;
	while (i < 300)
		used[i++] = 0;
	while (*set)
		used[(unsigned char)*set++] = 1;
	start = -1;
	while (s1[++start])
		if (!used[(unsigned char)s1[start]])
			break ;
	j = ft_strlen(s1);
	while (--j > start)
		if (!used[(unsigned char)s1[j]])
			break ;
	j = j - start + 1;
	return (ft_substr(s1, start, j));
}
