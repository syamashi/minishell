/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_sighander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 01:53:26 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/16 02:01:03 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern sig_atomic_t	g_intflag;

void	sh_input_inthandler()
{
	ft_putstr_fd("\b\b  \b\n", 2);
	ft_putstr_fd(PROMPT_NAME, 2);
	g_intflag = 1;
}

/*
**  GNL, ^\ not display
*/

void	sh_input_quithandler()
{
	ft_putstr_fd("\b\b  \b\b", 2);
}

/*
**  bash-3.2$ cat
**  ^\Quit: 3
**  bash-3.2$ echo $?
**  131
*/

void	sh_quithandler(int sig)
{
	ft_putstr_fd("Quit: ", 2);
	ft_putnbr_fd(sig, 2);
	ft_putstr_fd("\n", 2);
}

/*
**  cat ctrl+C
*/

void	sh_putendl_handler()
{
	ft_putstr_fd("\b\b  \b\n", 2);
}
