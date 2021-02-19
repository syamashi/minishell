/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:17:38 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/24 22:34:07 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t number, size_t size)
{
	void			*addr;
	unsigned int	n;

	n = number * size;
	addr = malloc(n);
	if (addr == NULL)
		return (NULL);
	if (addr)
		ft_bzero(addr, n);
	return (addr);
}
