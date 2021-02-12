/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 01:26:05 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/12 11:34:56 by syamashi         ###   ########.fr       */
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
** 1. open ENV
** 2. del ESC
** 3. del QUOTES
** 4. ft_strtrim
** 5. ambiguous check
*/

void	filename_make(char **filename, char *src, t_minishell *m_sh)
{
	int		i;
	int		j;
	t_list	*packs;
	char	*tmp;

	packs = ft_strtoken(src);
	env_expand(&packs, m_sh);
	quote_del(&packs);
	strs_join(&packs);
	if (!(*filename = ft_strdup(((t_pack *)packs->content)->line)))
		exit(ft_error("", 1));
	ft_lstclear(&packs, pack_free);
	tmp = *filename;
	*filename = ft_strtrim(*filename, " \t");
	free(tmp);
	tmp = NULL;
}

int	dir_error(char *filename, int n)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	return (n);
}

/*
** $abc
** start read "abc"
*/

char	*key_get(char *line)
{
	int i;

	i = 0;
	while (!is_keyend(line[i]))
		i++;
	return (ft_substr(line, 0, i));
}

/*
**  $option is not supported.
**  $$ = $null + $null
*/

char	*value_get(char	*key, t_minishell *m_sh)
{
	int		len;
	t_list	*mov;
	char	*dkey;
	
	if (!ft_strncmp(key, "?", 2))
		return (ft_itoa(m_sh->exit_status));
	else
	{
		len = ft_strlen(key);
		if (len == 0)
			return (ft_strdup(""));
		mov = m_sh->env_list;
		while (mov)
		{
			dkey = ((t_dict *)mov->content)->key;
			if (!ft_strncmp(key, dkey, len + 1))
				return (ft_strdup(((t_dict *)mov->content)->value));
			mov = mov->next;
		}
	}
	return (ft_strdup(""));
}

bool	envcheck_solve(char **value)
{
	char	*tmp;
	int		i;

	tmp = *value;
	if (!(*value = ft_strtrim(*value, " \t")))
		exit(ft_error("", 1));
	free(tmp);
	i = -1;
	while ((*value)[++i])
		if ((*value)[i] == ' ')
			return (true);
	return (false);
}

bool	redirect_envcheck(char *line, t_minishell *m_sh)
{
	int 	i;
	char	*key;
	char	*value;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '$')
		{
			if (!(key = key_get(line + i + 1)))
				exit(ft_error("", 1));
			if (!(value = value_get(key, m_sh)))
				exit(ft_error("", 1));
			free(key);
			if (envcheck_solve(&value))
			{
				free(value);
				return (true);
			}
			free(value);
		}
	}
	return (false);
}

bool	ambiguous_check(char *str, t_minishell *m_sh)
{
	t_list	*packs;
	t_list	*mov;
	char	*line;
	int		type;

	packs = ft_strtoken(str);
	mov = packs;
	while (mov)
	{
		line = ((t_pack *)mov->content)->line;
		type = ((t_pack *)mov->content)->type;
		if (type == STR && redirect_envcheck(line, m_sh))
		{
			ft_lstclear(&packs, pack_free);
			return (true);
		}
		mov = mov->next;
	}
	ft_lstclear(&packs, pack_free);
	return (false);
}

void	fdin_change(t_exec **ex, const int n, char* filename)
{
	if (errno)
	{
		fd_error(filename, 2);
		(*ex)->error_flag = true;
	}
	if ((*ex)->fd_in != 0)
		close((*ex)->fd_in);
	(*ex)->fd_in = n;
}

void	fdout_change(t_exec **ex, const int n, char* filename)
{
	if (errno)
	{
		fd_error(filename, 2);
		(*ex)->error_flag = true;
	}
	if ((*ex)->fd_out != 1)
		close((*ex)->fd_out);
	(*ex)->fd_out = n;
}

void	fd_controller(t_exec **ex, t_list *dir, t_minishell *m_sh)
{
	t_list	*mov;
	int		type;
	char	*filename;

	mov = dir;
	while (mov && !(*ex)->error_flag)
	{
		type = ((t_pack *)mov->content)->type;
		mov = mov->next;
		if (ambiguous_check(((t_pack *)mov->content)->line, m_sh))
		{
			m_sh->exit_status = dir_error(((t_pack *)mov->content)->line, 1);
			(*ex)->error_flag = true;
			fdin_change(ex, 0, "");
			fdout_change(ex, 1, "");
			break;
		}
		filename_make(&filename, ((t_pack *)mov->content)->line, m_sh);
		if (type == RDIR)
			fdout_change(ex, open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0666), filename);
		else if (type == RRDIR)
			fdout_change(ex, open(filename, O_WRONLY|O_CREAT|O_APPEND, 0666), filename);
		else if (type == LDIR)
			fdin_change(ex, open(filename, O_RDONLY), filename);
		free(filename);
		mov = mov->next;
	}
}

/*       pipe      pipe
** ast[0] -> ast[1] -> ast[2]
**  -leaf
**   -str
**   -dir
**
**          pipe         pipe
** exlist[0] -> exlist[1] -> exlist[2]
**  -ex          -ex          -ex
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
