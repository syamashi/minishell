/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:48:27 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/15 21:15:35 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

extern sig_atomic_t	g_intflag;

void	sh_input_inthandler()
{
	ft_putstr_fd("\b\b  \n", 2);
	ft_putstr_fd(PROMPT_NAME, 2);
	g_intflag = 1;
}

/*
**  GNL, ^\ not display
*/

void	sh_input_quithandler()
{
	ft_putstr_fd("\b\b  \b\b", 2);
}

/*
**  bash-3.2$ cat
**  ^\Quit: 3
**  bash-3.2$ echo $?
**  131
*/

void	sh_quithandler(int sig)
{
	ft_putstr_fd("Quit: ", 2);
	ft_putnbr_fd(sig, 2);
	ft_putstr_fd("\n", 2);
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

void	check_commentout(char **line)
{
	int i;
	int quote_flag;

	quote_flag = 0;
	i = -1;
	while ((*line)[++i])
	{
		if ((*line)[i] == '\'')
			quote_flag ^= 1;
		if ((*line)[i] == '"')
			quote_flag ^= 2;
		if (!quote_flag && (*line)[i] == '#' && 
		(!i || (*line)[i - 1] == ' ' || (*line)[i - 1] == '\t'))
		{
			(*line)[i] = '\0';
			break;
		}
	}
}

char	*line_validcheck(char **line)
{
	char	*tmp;

	check_commentout(line);
	tmp = *line;
	if (!(*line = ft_strtrim(*line, " \t")))
		exit(ft_error("malloc failed", 1, STDERR));
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
	if (signal(SIGINT, sh_input_inthandler) == SIG_ERR)
		exit(ft_error("sigerror", 1, STDERR));
	if (signal(SIGQUIT, sh_input_quithandler) == SIG_ERR)
		exit(ft_error("sigerror", 1, STDERR));
	ft_putstr_fd(PROMPT_NAME, 2);
	while ((ret = get_next_line(0, &line)) == 0)
	{
		ft_putstr_fd("  \b\b", STDERR);
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
	if (g_intflag && !(g_intflag = 0))
		m_sh->exit_status = 1;
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		exit(ft_error("sigerror", 1, STDERR));
	//if (signal(SIGQUIT, sh_quithandler) == SIG_ERR)
	//	exit(ft_error("sigerror", 1, STDERR));
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
