/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 15:47:12 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 13:07:46 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

void	display_env(t_minishell *m_sh, t_exec *ex)
{
	t_list	*mov;

	mov = m_sh->env_list;
	while (mov)
	{
		if (((t_dict *)mov->content)->value)
		{
			ft_putstr_fd(((t_dict *)mov->content)->key, STDOUT);
			ft_putstr_fd("=", STDOUT);
			ft_putstr_fd(((t_dict *)mov->content)->value, STDOUT);
			ft_putstr_fd("\n", STDOUT);
		}
		mov = mov->next;
	}
}

int		sh_env(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;

	argv = exec->argv + 1;
	if (*argv)
	{
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd(*argv, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	else
		display_env(m_sh, exec);
	return (0);
}
