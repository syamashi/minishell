/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_semicolon.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 19:03:56 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/30 14:34:58 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	add_lines(t_list **lines, char *line, int j)
{
	char	*add;
	t_list	*new;

	if (!(add = ft_strdup(line + j)))
	{
		ft_lstclear(lines, free);
		return (ft_error("[add_lines] add malloc failed", 1));
	}
	if (!(new = ft_lstnew(add)))
	{
		ft_lstclear(lines, free);
		return (ft_error("[add_lines] new malloc failed", 1));
	}
	ft_lstadd_back(lines, new);
	return (0);
}

int			divide_semicolon(t_list **lines, char *line)
{
	int quote_flag = 0;
	int command_flag = 0;
	int i = -1;
	int j = 0;
	
	while (line[++i])
	{
		if (is_space(line[i]))
			continue;
		if (line[i] == '\'' && quote_flag != 2)
			quote_flag ^= 1;
		if (line[i] == '"' && quote_flag != 1)
			quote_flag ^= 2;
		if (line[i] == '\\' && !quote_flag)
		{
			if (line[++i] == '\0')
				return (ft_error("escape_end", 1));
			continue;
		}
		if (line[i] != ';')
			command_flag = 1;
		if (line[i] != ';' || quote_flag)
			continue;
		if (command_flag == 0)
		{
			ft_lstclear(lines, free);
			return (ft_syntax_error(";", 1));
		}
		command_flag = 0;
		line[i] = '\0';
		if (add_lines(lines, line, j))
			return (1);
		j = i + 1;
	}
	if (*(line + j))
		if (add_lines(lines, line, j))
			return (1);
	if (quote_flag)
	{
		ft_lstclear(lines, free);
		return (ft_error("quote_opened", 1));
	}
	return (0);
}
