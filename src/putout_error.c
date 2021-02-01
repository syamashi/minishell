/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putout_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:53:01 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/30 11:57:25 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_error(char *str, int	i)
{
	ft_putstr(str);
	ft_putstr("\n");	
	return (i);
}

int	ft_syntax_error(char *str, int i)
{
	ft_putstr("bash: syntax error near unexpected token '");
	ft_putstr(str);
	ft_putstr("'");
	ft_putstr("\n");
	return (i);
}

int	ft_avoid_error(char *str, int i)
{
	ft_putstr("∑(´；□；｀)oops... '");
	ft_putstr(str);
	ft_putstr("'");
	ft_putstr(" is now developing...");
	ft_putstr("\n");
	return (i);
}

int	ft_exit_error(char *str, int i)
{
	ft_putstr("bash: exit: ");
	ft_putstr(str);
	ft_putstr(": numeric argument required");
	ft_putstr("\n");
	return (i);
}
