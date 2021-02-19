/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putout_error2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 18:19:53 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/18 20:18:50 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

int		dir_error(char *path, int n)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	return (n);
}

int		ft_cd_error(char *path, int n)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (n);
}

void	shlvl_error(int depth)
{
	char	*str;
	
	if (!(str = ft_itoa(depth)))
		exit(ft_error("", 1));
	ft_putstr_fd("minishell: warning: shell level (", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(") too high, resetting to 1\n", 2);
	free(str);
}
