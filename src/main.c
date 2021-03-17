/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 12:29:16 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/17 19:27:40 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		main(int argc, char **argv, char **envp)
{
	if (argc == 1 && argv)
		minishell(envp);
	else
	{
		ft_putstr_fd(MINISHELL, STDERR);
		ft_putstr_fd(EM_TOO_MANY_ARG, STDERR);
	}
	return (0);
}
