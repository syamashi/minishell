/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 12:47:17 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/15 19:22:51 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"
#include "../includes/debug.h"

/*
 * void	minishell(char **envp);
 *
 * # list types
 * t_list<t_list<t_pack>>	commands, tmp;
 *   intermediate products of command
 * t_list<t_exec>			ex_list;
 *
 * all error should be handled in each function called this function
 * 	execpt parse error in div_commands
 */

bool	and_orflag(t_minishell mini_sh, int type)
{
	if (type == 0)
		return (true);
	if (type == DAND && !mini_sh.exit_status)
		return (true);
	if (type == DPIPE && mini_sh.exit_status)
		return (true);
	return (false);
}

void	sh_init(t_minishell *m_sh, char **envp)
{
	char	*strpwd;
	
	m_sh->env_list = NULL;
	m_sh->env_oldpwd = NULL;
	m_sh->env_pwd = NULL;
	m_sh->exit_status = 0;
	m_sh->fd_backup = NULL;
	m_sh->pwd_dslash = false;
	m_sh->pwds = NULL;
	env_init(envp, m_sh);
	if (!(m_sh->home_defvalue = value_get("HOME", m_sh)))
		if (!(m_sh->home_defvalue = ft_strdup("")))
			exit(ft_error("malloc failed", 1, STDERR));
	if (!(strpwd = value_get("PWD", m_sh)))
		exit(ft_error("malloc failed", 1, STDERR));
	m_sh->pwds = pwdlst_init(strpwd, 0);
	free(strpwd);
}

void	close_fd_all(t_minishell *m_sh, t_list *ex_list)
{
	if (((t_exec*)ex_list->content)->fd_in != 0)
		close(((t_exec*)ex_list->content)->fd_in);
	if (((t_exec*)ex_list->content)->fd_out != 1)
		close(((t_exec*)ex_list->content)->fd_out);
	if (((t_exec*)ex_list->content)->fd_err != 2)
		close(((t_exec*)ex_list->content)->fd_err);
/*
	t_list		*rtmp;
	t_redirect	*rd;


	while (m_sh->fd_backup){
		rd = (m_sh->fd_backup)->content;
		dup2(rd->backup, rd->rint);
//		printf("backup(%d, %d)\n", rd->backup, rd->rint);
		close(rd->backup);
		rtmp = m_sh->fd_backup->next;
//		printf("rtmp:%p, size:%d\n", rtmp, ft_lstsize(m_sh->fd_backup));
		ft_lstdelone(m_sh->fd_backup, free);
		m_sh->fd_backup = rtmp;
	}
*/
}

void	minishell(char **envp)
{
	t_minishell	mini_sh;
	char		*line;
	t_command	*commands;
	t_command	*tmp;
	t_list		*ex_list;

	sh_init(&mini_sh, envp);
	while (1)
	{
		if (!(line = sh_prompt(&mini_sh)))
			continue ;
		commands = div_commands(&mini_sh, line);
		free(line);
		while (commands)
		{
			ex_list = to_ex_list(&mini_sh, (t_list **)&(commands->content));
//			exlist_debug(ex_list);
			if (and_orflag(mini_sh, commands->and_or))
				sh_launch(&mini_sh, ex_list);
			close_fd_all(&mini_sh, ex_list);
			ft_lstclear(&ex_list, del_t_exec);
			tmp = commands->next;
			ft_clstdelone(commands, del_command);
			commands = tmp;
		}
	}
}
