/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsenew7_exinit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 01:26:05 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/08 15:41:03 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void ex_free(t_exec **ex)
{
//	printf("[ex_free] %s\n", ((t_pack *)(ptr))->line);
	char **tmp;

	tmp = (*ex)->argv;
	while (tmp)
	{
		tmp = (*ex)->argv + 1;
		free((*ex)->argv);
		(*ex)->argv = NULL;
	}
	free(*ex);
	*ex = NULL;
}

void	ex_def(t_exec **ex, const t_list *ast)
{
	int argc;
	int	i;

	argc = ft_lstsize(((t_leaf*)ast->content)->str);
	
	printf("[ex_def] argc:%d\n", argc);
	if (!(*ex = (t_exec *)malloc(sizeof(t_exec))))
		exit(ft_error("minishell: malloc failed", 1));
	if (!((*ex)->argv = (char **)malloc(sizeof(char *) * (argc + 1))))
		exit(ft_error("minishell: malloc failed", 1));
	i = -1;
	while (++i <= argc)
		(*ex)->argv[i] = NULL;
	(*ex)->envp = NULL;
	(*ex)->fd_in = 0;
	(*ex)->fd_out = 1;
}

void	argv_init(t_exec **ex, const t_list *str)
{
	t_list	*mov;
	int		i;

	mov = str;
	i = -1;
	while (mov)
	{
		++i;
		if (!((*ex)->argv[i] = ft_strdup(((t_pack *)str->content)->line)))
			exit(ft_error("", 1));
		mov = mov->next;
	}
}

void direnv_expand

int	dir_error(char *filename, int n)
{
	ft_putstr_fd("minishell: ");
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ambiguous redirect\n");
	return (n);
}

void	fd_controller(t_exec **ex, const t_list *dir)
{
	t_list *mov;
	int		type;
	char	*filename;

	mov = dir;
	while (mov)
	{
		type = ((t_pack *)mov->content)->type;
		direnv_expand(&filename, ((t_pack *)mov->content)->line)
		if (ambiguous_check(filename, ex))
		{
			dir_error(((t_pack *)mov->content)->line);
		};
		if (type == RDIR)
		{
			if ((*ex)->fd_out != 1)
				close(fd_out);
		}
		//ambiguous error
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

void	exlist_init(const t_list *ast, t_list **exlist)
{
	t_list	*mov;
	t_exec	*ex;
	t_list	*new;
	t_list	*exlist;

	mov = ast;
	exlist = NULL;
	while (mov)
	{
		ex_def(&ex, mov);
		argv_init(ex, ((t_leaf *)ast->content)->str);
		fd_controller(ex, ((t_leaf *)ast->content)->dir);
		ft_lstadd_back(&exlist, ex);
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

