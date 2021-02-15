/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 15:22:52 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/15 15:24:02 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/sh_launch.h"

int		sh_pwd(t_minishell *m_sh, t_exec *exec)
{
	char	*pwd;
	char	*buf;

	buf = NULL;
	errno = 0;
	if (!(pwd = getcwd(buf, 1024)))
	{
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	else
		ft_putendl_fd(pwd, exec->fd_out);
	free(pwd);
	return (0);
}
