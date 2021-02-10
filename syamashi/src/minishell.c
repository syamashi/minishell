/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 12:47:17 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/10 14:21:52 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

sig_atomic_t	g_intflag;

void	sh_inthandler()
{
	ft_putstr("\nminishell > ");
	g_intflag = 1;
}

char	*ft_lstjoin(t_list *lst)
{
	char	*str;
	char	*tmp;

	if (!lst)
		return (ft_strdup(""));
	str = ft_strdup(lst->content);
	while (lst->next)
	{
		tmp = ft_strjoin(str, lst->next->content);
		free(str);
		str = tmp;
		lst = lst->next;
	}
	return (str);
}

/*char	*sh_prompt()
{
	char	*line;
	t_list	*store;
	int		ret;

	store = NULL;
	signal(SIGINT, sh_inthandler);
	ft_putstr("minishell > ");
	while ((ret = get_next_line(0, &line)) == 0)
	{
		if (g_intflag)
			ft_lstclear(&store, free);
		g_intflag = 0;
		if (!store && !ft_strcmp(line, ""))
		{
			free(line);
			ft_lstclear(&store, free);
			exit(0);
		}
		ft_lstadd_back(&store, ft_lstnew(line));
	}
	ft_putstr(line);
	signal(SIGINT, SIG_DFL);
	if (g_intflag)
		ft_lstclear(&store, free);
	g_intflag = 0;
	//if (ret < 0)
		//sh_error();
	ft_lstadd_back(&store, ft_lstnew(line));
	line = ft_lstjoin(store);
	ft_lstclear(&store, free);
	return (line);
}*/

static	int	line_free(char **line, int i)
{
	if (line)
	{
		free(*line);
		*line = NULL;
	}
	return (i);
}


int	sh_prompt(char **line)
{
	int		ret;
	char	*tmp;

	*line = NULL;
	ft_putstr("minishell > ");
	while ((ret = get_next_line(0, line)) <= 0)
	{
		if (ret == -1)
		{
			line_free(line, 1);
			return (ft_error("minishell: [sh_prompt] GNL error", 1));
		}
		if (**line)
		{
			line_free(line, 1);
			ft_error("minishell: not blank", 1);
			return (1);
		}
		ft_putstr("exit\n");
		exit(0);
	}
	tmp = *line;
	if (!(*line = ft_strtrim(*line, " \t")))
		exit(ft_error("", 1));
	free(tmp);
	return (0);
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
	t_list *top;
	t_list *packs;
	char *line;
	int type;

	top = store;
	int top_cnt = 0;
	while (top)
	{
		printf("store[%d]:\n", top_cnt);
		packs = top->content;
		while (packs){
			line = ((t_pack *)packs->content)->line;
			type = ((t_pack *)packs->content)->type;
			printf("[%10s][%2d]\n", line, type);
			packs = packs->next;
		}
		printf("\n");
		top = top->next;
		top_cnt++;
	}
}

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

void	minishell(char **envp)
{
	char	*line;
	t_list	*store;
	t_list	*env;
	t_list	*top;
	t_list	*ast;
	t_list	*exlist;
	t_list	*extop;
	int		r;

	env = NULL;
	line = NULL;
	store = NULL;
	ast = NULL;
	exlist = NULL;
	r = 0;
	env_init(envp, &env);
	while (1)
	{
		signal(SIGINT, sh_inthandler);
		all_free(&line, &store, &ast, &exlist);
		g_intflag = 0;
		if (sh_prompt(&line))
			continue;
		store = ft_lstnew(ft_strtoken(line));
		if (r = input_check(store->content))
			continue;
		store_div(&store); // storeをlist化
		top = store;
		while (top)
		{
//			update_env(); //環境変数の更新。 export内で更新までするかな
			env_expand((t_list**)&top->content, &env, r);
			packs_trim((t_list **)&top->content);
			ast_init(&ast, (t_list**)&top->content);
//			debug(store);
//			ast_debug(ast);
			exlist_init(ast, &exlist, &env, &r);
			exlist_debug(exlist);
//			r = sh_launch(exlist);
			top = top->next;
			all_free(NULL, NULL, &ast, &exlist);
		}
//		debug(store);
	}
}

/*		while (semi)
		{
			int j = 0;
			t_list *exlist = (t_list*)semi->content;
			while (exlist)
			{
				printf("---exlist[%d]---\n", j++);
				printf("argv[");
				int i=0;
				while (((t_exec*)exlist->content)->argv[i]){
					printf("\"%s\"", ((t_exec*)exlist->content)->argv[i++]);
					if (((t_exec*)exlist->content)->argv[i])
						printf(",");
				}
				printf("]\n");
				printf("fd_in:%d\n", ((t_exec*)exlist->content)->fd_in);
				printf("fd_out:%d\n", ((t_exec*)exlist->content)->fd_out);
				exlist = exlist->next;
			}
			ft_putstr("\n");
			//	sh_lounch(); // < pipe単位にlistにした、t_execを渡す。
			semi = semi->next;
		}
*/
