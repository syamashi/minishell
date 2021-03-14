/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_cd2_linkdel.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 10:58:37 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/14 11:01:21 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

bool	is_linkdel(t_minishell *m_sh, char *argv)
{
	t_minishell	sh;
	char		*dir;

	if (*argv == '/')
		return (false);
	sh.pwd_dslash = false;
	sh.pwds = NULL;
	pwdlst_cpy(&sh, m_sh);
	pwdlst_update(&sh, argv, false);
	dir = pwds_str(&sh);
	ft_lstclear(&sh.pwds, free);
	if (chdir(dir))
	{
		free(dir);
		return (true);
	}
	free(dir);
	return (false);
}
