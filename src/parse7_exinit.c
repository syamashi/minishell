/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 01:26:05 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/12 18:45:28 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	ex_def(t_exec **ex, const t_list *ast, t_list *env)
{
	int argc;
	int envc;
	int i;

	argc = ft_lstsize(((t_leaf*)ast->content)->str);
	envc = ft_lstsize(env);
	if (!(*ex = (t_exec *)malloc(sizeof(t_exec))))
		exit(ft_error("minishell: malloc failed", 1));
	if (!((*ex)->argv = (char **)malloc(sizeof(char *) * (argc + 1))))
		exit(ft_error("minishell: malloc failed", 1));
	if (!((*ex)->envp = (char **)malloc(sizeof(char *) * (envc + 1))))
		exit(ft_error("minishell: malloc failed", 1));
	i = -1;
	while (++i <= argc)
		(*ex)->argv[i] = NULL;
	i = -1;
	while (++i <= envc)
		(*ex)->envp[i] = NULL;
	(*ex)->fd_in = 0;
	(*ex)->fd_out = 1;
	(*ex)->error_flag = false;
}

void	argv_init(t_exec **ex, t_list *str)
{
	t_list	*mov;
	int		i;

	mov = str;
	i = -1;
	while (mov)
	{
		++i;
		if (!((*ex)->argv[i] = ft_strdup(((t_pack *)mov->content)->line)))
			exit(ft_error("", 1));
		mov = mov->next;
	}
}

void	envp_init(t_exec **ex, t_list *env)
{
	char	*key;
	char	*value;
	t_list	*mov;
	char	*line;
	int		i;

	mov = env;
	i = -1;
	while (mov)
	{
		key = ((t_dict *)mov->content)->key;
		if (!(line = ft_strjoin(key, "=")))
			exit(ft_error("", 1));
		value = ((t_dict *)mov->content)->value;
		if (!((*ex)->envp[++i] = ft_strjoin(line, value)))
			exit(ft_error("", 1));
		free(line);
		line = NULL;
		mov = mov->next;
	}
}

/*
**  ast[0] -(pipe)- ast[1] -(pipe)- ast[2]
**  -leaf
**   -str
**   -dir
**
** exlist[0] -(pipe)- exlist[1] -(pipe)- exlist[2]
**  -ex
**   -argv[]
**   -envp[]
**   -fd_in
**   -fd_out
**   -errorflag
*/

void	exlist_init(t_list *ast, t_list **exlist, t_minishell *m_sh)
{
	t_list	*mov;
	t_exec	*ex;
	t_list	*new;

	mov = ast;
	*exlist = NULL;
	while (mov)
	{
		ex_def(&ex, mov, m_sh->env_list);
		argv_init(&ex, ((t_leaf *)mov->content)->str);
		envp_init(&ex, m_sh->env_list);
		fd_controller(&ex, ((t_leaf *)mov->content)->dir, m_sh);
		if (!(new = ft_lstnew(ex)))
			exit(ft_error("", 1));
		ft_lstadd_back(exlist, new);
		mov = mov->next;
	}
}
