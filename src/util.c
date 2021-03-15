/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 16:53:33 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 19:22:51 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

void	packinfo_get(char **line, int *type, const t_list *mov)
{
	*line = ((t_pack *)(mov)->content)->line;
	*type = ((t_pack *)(mov)->content)->type;
}

void	quoteflag_get(int type, int *quote_flag)
{
	if (type == SQUOTE && *quote_flag != 2)
		*quote_flag ^= 1;
	if (type == DQUOTE && *quote_flag != 1)
		*quote_flag ^= 2;
}

void	simplejoin(char **output, char *add)
{
	char	*tmp;

	tmp = *output;
	if (!(*output = ft_strjoin(*output, add)))
		exit(ft_error("malloc failed", 1, STDERR));
	free(tmp);
}

char	*simpletrim(char *output, char *trim)
{
	char	*tmp;

	tmp = output;
	if (!(output = ft_strtrim(output, trim)))
		exit(ft_error("malloc failed", 1, STDERR));
	free(tmp);
	return (output);
}
