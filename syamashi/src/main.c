/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 12:29:16 by ewatanab          #+#    #+#             */
/*   Updated: 2021/01/30 18:22:57 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		main(int argc, char **argv, char **envp)
{
	fflush(stdout); setbuf(stdout,NULL);
	if (argc == 1)
		minishell(envp);
	else
	{
		ft_putstr_fd(MINISHELL, 2);
		ft_putstr_fd(EM_TOO_MANY_ARG, 2);
	}
	return (0);
}
