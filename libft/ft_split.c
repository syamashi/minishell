/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 21:18:30 by syamashi          #+#    #+#             */
/*   Updated: 2020/07/01 21:52:49 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	find_s(const char *str, char c, int goal)
{
	int i;

	i = -1;
	while (str[goal + ++i] != '\0')
	{
		if (str[goal + i] == c)
			continue ;
		return (goal + i);
	}
	return (-1);
}

static int	find_g(const char *str, char c, int start)
{
	int i;

	if (start < 0)
		return (0);
	i = -1;
	while (str[start + ++i] != '\0')
	{
		if (str[start + i] == c)
			return (start + i);
	}
	return (start + i);
}

static int	wd_size(const char *str, char c)
{
	int count;

	count = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str)
			count++;
		while (*str && *str != c)
			str++;
	}
	return (count);
}

static void	*memleak(char **ans, int j)
{
	while (j--)
		free(ans[j]);
	free(ans);
	return (NULL);
}

char		**ft_split(char const *str, char c)
{
	char	**ans;
	int		j;
	int		start;
	int		goal;

	if (!str)
		return (NULL);
	j = -1;
	start = find_s(str, c, 0);
	goal = find_g(str, c, start);
	if (!(ans = (char **)malloc(sizeof(char *) * (wd_size(str, c) + 1))))
		return (NULL);
	while (start > -1)
	{
		if (!(ans[++j] = ft_substr(str, start, goal - start)))
			return (memleak(ans, j));
		start = find_s(str, c, goal);
		if (start == -1)
			break ;
		goal = find_g(str, c, start);
	}
	ans[++j] = NULL;
	return (ans);
}
