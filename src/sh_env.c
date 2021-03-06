/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 15:47:12 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 19:40:56 by syamashi         ###   ########.fr       */
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
			ft_putstr_fd(((t_dict *)mov->content)->key, ex->fd_out);
			ft_putstr_fd("=", ex->fd_out);
			ft_putstr_fd(((t_dict *)mov->content)->value, ex->fd_out);
			ft_putstr_fd("\n", ex->fd_out);
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
		ft_putstr_fd("env: ", exec->fd_err);
		ft_putstr_fd(*argv, exec->fd_err);
		ft_putstr_fd(": No such file or directory\n", exec->fd_err);
		return (127);
	}
	else
		display_env(m_sh, exec);
	return (0);
}
