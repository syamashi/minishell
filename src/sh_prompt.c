/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:48:27 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/10 18:06:28 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern sig_atomic_t	g_intflag;

void	sh_inthandler()
{
	ft_putstr("\n");
	ft_putstr(PROMPT_NAME);
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

void	sh_exit(int status)
{
	exit(0);
}

char	*sh_prompt()
{
	char	*line;
	t_list	*store;
	int		ret;

	store = NULL;
	signal(SIGINT, sh_inthandler);
	ft_putstr(PROMPT_NAME);
	while ((ret = get_next_line(0, &line)) == 0)
	{
		if (g_intflag)
			ft_lstclear(&store, free);
		g_intflag = 0;
		if (!store && !ft_strcmp(line, ""))
		{
			free(line);
			ft_putstr("exit\n");
			exit(0);
		}
		ft_lstadd_back(&store, ft_lstnew(line));
	}
	signal(SIGINT, SIG_DFL);
	//if (g_intflag)
	//	ft_lstclear(&store, free);
	//g_intflag = 0;
	//if (ret < 0)
		//sh_error();
	ft_lstadd_back(&store, ft_lstnew(line));
	line = ft_lstjoin(store);
	ft_lstclear(&store, free);
	return (line);
}

/*
int		main()
{
	char	*line;

	while (1)
	{
		line = sh_prompt();
		ft_putstr(line);
		ft_putstr("\n");
	}
}
*/
