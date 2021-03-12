/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:48:27 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/12 18:26:08 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

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

char	*line_validcheck(char **line)
{
	char *tmp;
	
	tmp = *line;
	if (!(*line = ft_strtrim(*line, " \t")))
		exit(ft_error("malloc failed", 1));
	free(tmp);
	tmp = NULL;
	if (**line == '\0')
	{
		free(*line);
		*line = NULL;
		return (NULL);
	}
	return (*line);
}

char	*sh_prompt(t_minishell *m_sh)
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
			ft_putstr_fd("exit\n", 2);
			exit(m_sh->exit_status);
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
	return (line_validcheck(&line));
}
