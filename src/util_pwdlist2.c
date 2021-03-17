/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_pwdlist2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 12:16:20 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/17 19:34:57 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

void	lstadd_str(t_list **ret, char *str)
{
	t_list	*new;
	char	*add;

	if (!(add = ft_strdup(str)))
		exit(ft_error("malloc failed", 1, STDERR));
	if (!(new = ft_lstnew(add)))
		exit(ft_error("malloc failed", 1, STDERR));
	ft_lstadd_back(ret, new);
}

void	lstadd_substr(t_list **ret, char *str, int start, int len)
{
	t_list	*new;
	char	*add;

	if (!(add = ft_substr(str, start, len)))
		exit(ft_error("malloc failed", 1, STDERR));
	if (!(new = ft_lstnew(add)))
		exit(ft_error("malloc failed", 1, STDERR));
	ft_lstadd_back(ret, new);
}

t_list	*pwdlst_nocurrent(char *str)
{
	t_list	*ret;
	t_list	*new;
	char	*add;
	int		i;

	i = -1;
	ret = NULL;
	while (str[++i])
	{
		if (str[i] == '.')
			add = ft_strdup(".");
		else if (i == 0 || str[i - 1] == '/')
			add = ft_strdup("");
		else
			continue ;
		if (!add || !(new = ft_lstnew(add)))
			exit(ft_error("malloc failed", 1, STDERR));
		ft_lstadd_back(&ret, new);
	}
	if (i && str[i - 1] == '/')
		lstadd_str(&ret, "");
	return (ret);
}

t_list	*pwdlst_solve(char *str)
{
	t_list	*ret;
	int		i;
	int		j;

	i = -1;
	j = 0;
	ret = NULL;
	while (str[++i])
	{
		while (str[i] && str[i] != '/')
			i++;
		if (i > j)
			lstadd_substr(&ret, str, j, i - j);
		j = i + 1;
		if (!str[i])
			i--;
	}
	return (ret);
}

t_list	*pwdlst_init(char *str, int delflag)
{
	if (delflag == 1)
		return (pwdlst_nocurrent(str));
	else
		return (pwdlst_solve(str));
}
