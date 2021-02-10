/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_exinit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 01:26:05 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/10 23:02:23 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	ex_def(t_exec **ex, const t_list *ast, t_list **env)
{
	int argc;
	int envc;
	int i;

	argc = ft_lstsize(((t_leaf*)ast->content)->str);
	envc = ft_lstsize(*env);
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

void	envp_init(t_exec **ex, t_list **env)
{
	char	*key;
	char	*value;
	t_list	*mov;
	char	*line;
	int		i;

	mov = *env;
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

void	filename_make(char **filename, char *src, t_list **env, int *r)
{
	int		i;
	int		j;
	t_list	*packs;
	char	*tmp;

	packs = ft_strtoken(src);
	env_expand(&packs, env, *r);
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

bool	ambiguous_check(const char *filename)
{
	int i;

	i = -1;
	while (filename[++i])
	{
		if (filename[i] == ' ')
			return (true);
	}
	return (false);
}

void	fd_error(char *str, int fd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	errno = 0;
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

void	fd_controller(t_exec **ex, t_list *dir, t_list **env, int *r)
{
	t_list	*mov;
	int		type;
	char	*filename;

	mov = dir;
	while (mov && !(*ex)->error_flag)
	{
		type = ((t_pack *)mov->content)->type;
		mov = mov->next;
		filename_make(&filename, ((t_pack *)mov->content)->line, env, r);
		if (ambiguous_check(filename))
		{
			*r = dir_error(((t_pack *)mov->content)->line, 1);
			(*ex)->error_flag = true;
			fdin_change(ex, 0, filename);
			fdout_change(ex, 1, filename);
		}
		else if (type == RDIR)
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
*/

void	exlist_init(t_list *ast, t_list **exlist, t_list **env, int *r)
{
	t_list	*mov;
	t_exec	*ex;
	t_list	*new;

	mov = ast;
	*exlist = NULL;
	while (mov)
	{
		ex_def(&ex, mov, env);
		argv_init(&ex, ((t_leaf *)mov->content)->str);
		envp_init(&ex, env);
		fd_controller(&ex, ((t_leaf *)mov->content)->dir, env, r);
		if (!(new = ft_lstnew(ex)))
			exit(ft_error("", 1));
		ft_lstadd_back(exlist, new);
		mov = mov->next;
	}
}

/*
int pack_toexec(t_list **ast, t_list *pack)
{
	int		type;
	char	*line;
	t_exec	*ex;
	t_list	*new;
	t_list	*exlist;
	int		i;

	if (def_ex(&ex, pack))
		return (1);
//	printf("[pack_toexec] fd_out:%d\n", ex->fd_out);
	exlist = NULL;
	i = 0;
	while (pack)
	{
		// astのlistにexのlistをつけていく
		if (((t_pack *)pack->content)->type == 1) //pipe来たらnew ex
		{
			i = 0;
			if (!(new = ft_lstnew(ex)))
			{
				ex_free(&ex);
				ft_lstclear(&exlist, exlist_free);
				return (ft_error("[pack_toexec] new malloc error", 1));
			}
			ft_lstadd_back(&exlist, new);
			if (def_ex(&ex, pack->next))
				return (1);
			pack = pack->next;
		}
		else if (((t_pack *)pack->content)->type == 2)
		{
			if (!(ft_strncmp(((t_pack *)pack->content)->line, "<", 2)))
			{
				if (ex->fd_in != -1)
					close(ex->fd_in);
				ex->fd_in = open(((t_pack *)pack->next->content)->line, O_RDONLY);
			}
			if (!(ft_strncmp(((t_pack *)pack->content)->line, ">", 2)))
			{
				if (ex->fd_out != -1)
					close(ex->fd_out);
				ex->fd_out = open(((t_pack *)pack->next->content)->line, O_WRONLY|O_CREAT|O_TRUNC, 0666);
			}
			if (!(ft_strncmp(((t_pack *)pack->content)->line, ">>", 3)))
			{
				if (ex->fd_out != -1)
					close(ex->fd_out);
				ex->fd_out = open(((t_pack *)pack->next->content)->line, O_WRONLY|O_CREAT|O_APPEND, 0666);
			}
			pack = pack->next->next;
		} // リダイレクトきたらskip
		else if (!(ft_strncmp(((t_pack *)pack->content)->line, "-n", 3))
			&& ex->argv[0] //echoコマンドの直後なら
			&& !(ft_strncmp(ex->argv[0], "echo", 5))
			&& !ex->argv[1])
		{
			pack = pack->next;
		}
		else // string
		{
			if (!(ex->argv[i++] = ft_strdup(((t_pack *)pack->content)->line)))
			{
				ex_free(&ex);
				ft_lstclear(&exlist, exlist_free);
				return (ft_error("[pack_toexec] ex->argv[] malloc error", 1));
			}
//			printf("[pack_toexec] argv[%d]:%s\n", i-1, ex->argv[i-1]);
			pack = pack->next;
		}
	}
	if (!(new = ft_lstnew(ex)))
	{
		ex_free(&ex);
		ft_lstclear(&exlist, exlist_free);
		return (ft_error("[pack_toexec] new malloc error", 1));
	}
	ft_lstadd_back(&exlist, new);
//	printf("[pack_toexec] exlistsize:%d\n", ft_lstsize(exlist));
	if (!(new = ft_lstnew(exlist)))
	{
		ex_free(&ex);
		ft_lstclear(&exlist, exlist_free);
		return (ft_error("[pack_toexec] ft_lstnew(exlist) malloc error", 1));
	}
	ft_lstadd_back(ast, new);
//	printf("[pack_toexec] fd_out:%d\n", ((t_exec*)new->content)->fd_out);
	return (0);
}
*/

