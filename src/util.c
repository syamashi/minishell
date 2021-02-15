/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 16:53:33 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/15 18:06:27 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

bool	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

bool	is_quote(int n)
{
	return (n == SQUOTE ||
			n == DQUOTE);
}

bool	is_meta(char c)
{
	return (c == '\'' ||
			c == '\"' ||
			c == '&' ||
			c == '<' ||
			c == '>' ||
			c == '|' ||
			c == '\\' ||
			c == ';');
}

bool	is_envmeta(char c)
{
	return (c == '\'' ||
			c == '\"' ||
			c == '&' ||
			c == '<' ||
			c == '>' ||
			c == '|' ||
			c == ';');
}

bool	is_keyend(char c)
{
	return (is_space(c) ||
			c == '=' ||
			c == '$' ||
			c == '\0');
}

bool	is_dir(int n)
{
	return (n == RDIR ||
			n == RRDIR ||
			n == LDIR ||
			n == LLDIR ||
			n == LLLDIR);
}

bool	is_metatype(int n)
{
	return (n == PIPE ||
			n == DPIPE ||
			n == AND ||
			n == DAND ||
			n == SCOLON);
}

bool	is_bonus(int n)
{
	return (n == LLDIR ||
			n == LLLDIR ||
			n == DPIPE ||
			n == AND ||
			n == DAND);
}

bool	isnot_cmd(const int type)
{
	return (type == SPACE ||
			type == SQUOTE ||
			type == DQUOTE);
}

bool	is_cmd(int type)
{
	return (type == STR ||
			is_dir(type));
}

bool	is_esc(char c)
{
	return (c == '"' ||
			c == '\\' ||
			c == '`' ||
			c == '$');
}

/*
** $abc
** start read "abc"
*/

char	*key_get(char *line)
{
	int i;

	i = 0;
	while (!is_keyend(line[i]))
		i++;
	return (ft_substr(line, 0, i));
}

/*
**  $option is not supported.
**  $$ = $null + $null
*/

char	*value_get(char	*key, t_minishell *m_sh)
{
	int		len;
	t_list	*mov;
	char	*dkey;

	if (!ft_strncmp(key, "?", 2))
		return (ft_itoa(m_sh->exit_status));
	else
	{
		len = ft_strlen(key);
		if (len == 0)
			return (ft_strdup(""));
		mov = m_sh->env_list;
		while (mov)
		{
			dkey = ((t_dict *)mov->content)->key;
			if (!ft_strncmp(key, dkey, len + 1))
				return (ft_strdup(((t_dict *)mov->content)->value));
			mov = mov->next;
		}
	}
	return (ft_strdup(""));
}

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
		exit(ft_error("", 1));
	free(tmp);
}

char	*simpletrim(char *output, char *trim)
{
	char	*tmp;

	tmp = output;
	if (!(output = ft_strtrim(output, trim)))
		exit(ft_error("", 1));
	free(tmp);
	return (output);
}
