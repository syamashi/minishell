/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 15:47:12 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/14 22:05:03 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/sh_launch.h"

void	display_env(t_minishell *m_sh)
{
	t_list	*mov;

	mov = m_sh->env_list;
	while (mov)
	{
		ft_putstr_fd(((t_dict *)mov->content)->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(((t_dict *)mov->content)->value, 1);
		ft_putstr_fd("\n", 1);
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
		display_env(m_sh);
	return (0);
}
