/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 01:26:05 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/30 19:33:51 by syamashi         ###   ########.fr       */
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

static int arglen(t_list *pack)
{
	int		type;
	char	*line;
	int		size;
	int		command_flag;

// リダイレクトがきたら、2個とばす
// echoのあと、-nがきたらとばす
// pipeがきたらbreak
// 最初がリダイレクトだったら、そうじゃなくなるまでとばす
// echoだったら、
	size = 0;
	command_flag = 0; // 最初に文字列きたら1。echoだったら2。
	while (pack)
	{
		if (((t_pack *)pack->content)->type == 1)
			break;
		else if (((t_pack *)pack->content)->type == 2)
			pack = pack->next;
		else if (((t_pack *)pack->content)->type == 0)
		{
			if (command_flag == 0)
			{
				if (!(ft_strncmp(((t_pack *)pack->content)->line, "echo", 5)))
					command_flag = 2;
				else
					command_flag = 1;
				size++;
			}
			else if (command_flag == 1)
				size++;
			else if (command_flag == 2)
			{
				if (ft_strncmp(((t_pack *)pack->content)->line, "-n", 3))
				{
					command_flag = 1;
					size++;
				}
			}
		}
		pack = pack->next;
	}
	return (size);
}

int	def_ex(t_exec **ex, t_list *pack)
{
	int argc;
	int	i;

	argc = arglen(pack);
//	printf("[def_ex] argc:%d\n", argc);
	if (!(*ex = (t_exec *)malloc(sizeof(t_exec))))
		return (ft_error("[def_ex] ex malloc failed", 1));
	if (!((*ex)->argv = (char **)malloc(sizeof(char *) * (argc + 1))))
	{
		free(ex);
		ex = NULL;
		return (ft_error("[def_ex] (*ex)->argv malloc failed", 1));
	}
	i = -1;
	while (++i <= argc)
		(*ex)->argv[i] = NULL;
	(*ex)->envp = NULL;
	(*ex)->fd_in = -1;
	(*ex)->fd_out = -1;
	(*ex)->echo_option = 0;
	return (0);
}

int pack_toexec(t_list **store, t_list *pack)
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
		// storeのlistにexのlistをつけていく
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
			ex->echo_option = 1;
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
	ft_lstadd_back(store, new);
//	printf("[pack_toexec] fd_out:%d\n", ((t_exec*)new->content)->fd_out);
	return (0);
}

int get_exec(t_list **store, t_list **packs)
{
	t_list	*semi;

	semi = *packs;
	while (semi)
	{
		if (pack_toexec(store, semi->content)) //storeにexのlistをつけたしていく
		{
			ft_lstclear(packs, pack_free);
			return (1);
		}
		semi = semi->next;
	}
	return (0);
}
