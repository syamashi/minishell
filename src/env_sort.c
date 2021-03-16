/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 01:47:57 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/16 20:18:05 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

static t_list	*concatante(t_list *a, t_list *b)
{
	t_list *p;

	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);
	p = a;
	ft_lstadd_back(&p, b);
	return (a);
}

static void		distribution(t_list **c, t_list **piv)
{
	(*c)->next = *piv;
	*piv = *c;
}

static void		def_pivots(t_list **l_pivot, t_list **e_pivot, t_list **r_pivot)
{
	*l_pivot = NULL;
	*e_pivot = NULL;
	*r_pivot = NULL;
}

t_list			*quick_sort_list(t_list *c)
{
	char	*pivot;
	t_list	*t;
	t_list	*l_pivot;
	t_list	*e_pivot;
	t_list	*r_pivot;

	if (!c || !c->next)
		return (c);
	def_pivots(&l_pivot, &e_pivot, &r_pivot);
	pivot = ((t_dict *)c->content)->key;
	while (c)
	{
		t = c->next;
		if (ft_strcmp(pivot, ((t_dict*)c->content)->key) > 0)
			distribution(&c, &l_pivot);
		else if (!ft_strcmp(pivot, ((t_dict*)c->content)->key))
			distribution(&c, &e_pivot);
		else
			distribution(&c, &r_pivot);
		c = t;
	}
	l_pivot = quick_sort_list(l_pivot);
	r_pivot = quick_sort_list(r_pivot);
	return (concatante(l_pivot, concatante(e_pivot, r_pivot)));
}
