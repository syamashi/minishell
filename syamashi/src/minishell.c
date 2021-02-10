/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 12:47:17 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/10 16:18:39 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"
#include "../includes/debug.h"

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
