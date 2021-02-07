/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 12:47:17 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/07 20:34:50 by ewatanab         ###   ########.fr       */
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

static char	*get_line(t_list **env)
{
	char	*line;
	int		ret;

	line = NULL;
	while ((ret = get_next_line(0, &line)) <= 0)
	{
		if (ret == -1)
		{
			line_free(&line, 1);
			all_free(NULL, env);
			ft_error("[sh_prompt] GNL error", 1);
			exit(2);
		}
		if (*line)
		{
			line_free(&line, 1);
			ft_error(" : not blank", 1);
			return (NULL);
		}
		line_free(&line, 1);
		ft_putstr("exit\n");
		all_free(NULL, env);
		exit(0);
	}
	return (line);
}

int	sh_prompt(t_list **store, t_list **env)
{
	char	*line;
	t_list	*lines;
	t_list  *packs;

	line = NULL;
	lines = NULL;
	packs = NULL;
	ft_putstr("minishell > ");
	if (!(line = get_line(env)))
		return (1);
	if (divide_semicolon(&lines, line))
		return (line_free(&line, 1));
	line_free(&line, 0);
	if (expand_env(&lines, env))
		return (1);
	if (packed(&packs, &lines))
		return (1);
	ft_lstclear(&lines, free);
	if (meta_check(&packs))
		return (1);
	if (get_exec(store, &packs))
		return (1);
	packs_free(&packs);
	return (0);
}

void	minishell(char **envp)
{
	t_list		*env;
	t_list		*semi;
	t_list		*store; // 外側のlistはセミコロン単位。その中に、pipe単位のt_exec。
	int			r;

	env = NULL;
	store = NULL;
	r = init_env(envp, &env);
	while (1)
	{
		signal(SIGINT, sh_inthandler);
		all_free(&store, NULL);
		g_intflag = 0;
		if ((r = sh_prompt(&store, &env))) //ctrl+D exitする可能性ある
			continue;
		semi = store;
		while (semi)
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
			sh_lounch(semi->content); // < pipe単位にlistにした、t_execを渡す。
			semi = semi->next;
		}
	}
}
