/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:40:05 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/11 21:07:22 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/debug.h"

void	exlist_debug(t_list *exlist)
{
	t_list *top;
	t_exec *ex;

	top = exlist;
	int top_cnt = 0;
	while (top)
	{
		printf("-----exlist[%d]-----\n", top_cnt);
		ex = (t_exec *)top->content;
		int j = -1;
		printf("argv  :{");
		while (ex->argv[++j])
		{
			printf("%s",ex->argv[j]);
			if (ex->argv[j+1])	printf(", ");
		}
		printf("}\n");
/*		printf("envp[]:");
		j = -1;
		while (ex->envp[++j])
		{
			printf("[%s]",ex->envp[j]);
			if (ex->envp[j+1])	printf(",");
		}
		printf("\n");
*/		printf("fd_in :%d\n", ex->fd_in);
		printf("fd_out:%d\n", ex->fd_out);
		printf("error_flag:%d\n", ex->error_flag);
		top = top->next;
		top_cnt++;
	}
}

void	ast_debug(t_list *ast)
{
	t_list *str;
	t_list *dir;
	char	*line;
	int		type;
	int ast_cnt = 0;
	while (ast)
	{
		printf("-----ast[%d]-----\n", ast_cnt++);
		str = ((t_leaf *)ast->content)->str;
		dir = ((t_leaf*)ast->content)->dir;
		int str_cnt = 0;
		int dir_cnt = 0;
//	printf("strsize:%d, dirsize:%d\n", ft_lstsize(str), ft_lstsize(dir));
		while (str)
		{
			printf("str[%d]:", str_cnt);
			line = ((t_pack *)str->content)->line;
			type = ((t_pack *)str->content)->type;
				printf("[%10s][%2d]\n", line, type);
			str_cnt++;
			str = str->next;
		}
		while (dir)
		{
			printf("dir[%d]:", dir_cnt);
			line = ((t_pack *)dir->content)->line;
			type = ((t_pack *)dir->content)->type;
			printf("[%10s][%2d]\n", line, type);
			dir_cnt++;
			dir = dir->next;
		}
		ast = ast->next;
	}
}

void	debug(t_list *store)
{
	t_list *packs;
	char *line;
	int type;

	packs = store;
	while (packs){
		line = ((t_pack *)packs->content)->line;
		type = ((t_pack *)packs->content)->type;
		printf("[%10s][%2d]\n", line, type);
		packs = packs->next;
	}
	printf("\n");
}
