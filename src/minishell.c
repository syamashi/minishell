/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 12:47:17 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/11 21:22:49 by syamashi         ###   ########.fr       */
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
	env_init(envp, &(m_sh->env_list));
}

t_list	*div_commands(t_minishell *m_sh, char *line)
{
	t_list	*store;

	store = ft_lstnew(ft_strtoken(line));
	if ((m_sh->exit_status = input_check(store->content)))
		return (NULL);
	store_div(&store);
	return (store);
}

// why recieve double pointer list???????
t_list	*to_ex_list(t_minishell *m_sh, t_list **pack_list)
{
	t_list	*ex_list;
	t_list	*ast;
	t_list	*env;

	ast = NULL;
	ex_list = NULL;
	env = m_sh->env_list;
//	debug(*pack_list);
	env_expand((t_list**)pack_list, m_sh);
	packs_trim((t_list **)pack_list);
	ast_init(&ast, (t_list**)pack_list);
	ast_debug(ast);
	exlist_init(ast, &ex_list, m_sh);
	ast_free(&ast);
	exlist_debug(ex_list);
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

void	minishell(char	**envp)
{
	t_minishell	mini_sh;
	char		*line;
	t_list		*commands;
	t_list		*tmp;
	t_list		*ex_list;

	sh_init(&mini_sh, envp);
	while (1)
	{
		line = sh_prompt(&mini_sh);
		commands = div_commands(&mini_sh, line);
		free(line);
		while (commands)
		{
			ex_list = to_ex_list(&mini_sh, (t_list **)&(commands->content));
			sh_launch(&mini_sh, ex_list);
			ft_lstclear(&ex_list, del_t_exec);
			tmp = commands->next;
			ft_lstdelone(commands, del_command);
			commands = tmp;
		}
	}
}
