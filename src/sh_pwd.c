/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 15:22:52 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 17:59:16 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

int		sh_pwd(t_minishell *m_sh, t_exec *exec)
{
	char	*pwd;
	char	*buf;

	pwd = pwds_str(m_sh);
	ft_putendl_fd(pwd, exec->fd_out);
	free(pwd);
	return (0);
}
