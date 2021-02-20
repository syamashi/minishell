/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 12:47:17 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/20 16:10:08 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"
#include "../includes/debug.h"

void	ft_vlstclear(void *vlst_arg, void (*del)(void*))
{
	void	**vlst;
	void	**it;

	vlst = vlst_arg;
	it = vlst;
	while (*it)
		del(*it++);
	free(vlst);
}

void	del_t_exec(void *exec_arg)
{
	t_exec	*exec;

	exec = exec_arg;
	ft_vlstclear(exec->argv, free);
	ft_vlstclear(exec->envp, free);
	free(exec);
}

void	del_pack(void *pack_arg)
{
	t_pack *pack;

	pack = pack_arg;
	free(pack->line);
	free(pack);
}

void	del_command(void *pack_list_arg)
{
	t_list	*pack_list;

	pack_list = pack_list_arg;
	ft_lstclear(&pack_list, del_pack);
}

void	sh_init(t_minishell *m_sh, char **envp)
{
	m_sh->env_list = NULL;
	m_sh->exit_status = 0;
	env_init(envp, m_sh);
	if (!(m_sh->home_defvalue = value_get("HOME", m_sh)))
		if (!(m_sh->home_defvalue = ft_strdup("")))
			exit(ft_error("minishell: malloc failed", 1));
	m_sh->env_list = quick_sort_list(m_sh->env_list);
}

t_command	*div_commands(t_minishell *m_sh, char *line)
{
	t_command	*store;

	store = ft_clstnew(ft_strtoken(line));
	if (input_check(store->content, m_sh))
		return (NULL);
	store_div(&store);
	return (store);
}

bool	is_tilde(char *line)
{
	if (ft_strncmp(line, "~/", 2) && ft_strncmp(line, "~", 2))
		return (false);
	return (true);
}

void	tilde_join(t_list *mov, t_minishell *m_sh)
{
	char	*line;
	char	*tmp;
	char	*home_value;

	line = ((t_pack*)mov->content)->line;
	if (!(home_value = value_get("HOME", m_sh)))
		if (!(home_value = ft_strdup(m_sh->home_defvalue)))
			if (!(home_value = ft_strdup("")))
				exit(ft_error("minishell: malloc failed", 1));
	tmp = line;
	line = ft_strjoin(home_value, line + 1);
	free(tmp);
	tmp = NULL;
	free(home_value);
	home_value = NULL;
	((t_pack *)mov->content)->line = line;
}

void	tilde_expand(t_list **pack_list, t_minishell *m_sh)
{
	t_list	*mov;
	char	*line;
	int		type;
	int		pre_type;

	mov = *pack_list;
	pre_type = SPACE;
	while (mov)
	{
		packinfo_get(&line, &type, mov);
		if (pre_type == SPACE && type == STR && is_tilde(line))
			tilde_join(mov, m_sh);
		pre_type = type;
		mov = mov->next;
	}
}

// why recieve double pointer list???????
t_list	*to_ex_list(t_minishell *m_sh, t_list **pack_list)
{
	t_list	*ex_list;
	t_list	*ast;

	ast = NULL;
	ex_list = NULL;
	tilde_expand(pack_list, m_sh);
//	debug(*pack_list);
	env_expand((t_list **)pack_list, m_sh, 0);
//	debug(*pack_list);
	packs_trim((t_list **)pack_list);
//	debug(*pack_list);
	ast_init(&ast, (t_list**)pack_list);
	//ast_debug(ast);
	exlist_init(ast, &ex_list, m_sh);
//	debug(*pack_list);
	ast_free(&ast);
	//exlist_debug(ex_list);
	return (ex_list);
}

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
			// semiなら実行。&&なら、ret==0, ||ならret>0
			if (and_orflag(mini_sh, commands->and_or))
				sh_launch(&mini_sh, ex_list);
			ft_lstclear(&ex_list, del_t_exec);
			tmp = commands->next;
			ft_clstdelone(commands, del_command);
			commands = tmp;
		}
	}
}
