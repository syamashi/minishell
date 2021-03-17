/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 15:22:52 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/17 19:33:49 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

int		sh_pwd(t_minishell *m_sh, t_exec *exec)
{
	char	*pwd;

	pwd = pwds_str(m_sh);
	ft_putendl_fd(pwd, exec->fd_out);
	free(pwd);
	return (0);
}
