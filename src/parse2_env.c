/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 22:00:44 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/31 16:53:48 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int env_join(char **new, t_list **env, char *line, int *i)
{
	char	*key;
	char	*tmp;
	t_list	*pos;
	int 	j;
	int		len;

	++*i;
	j = *i;
//	printf("[env_join] i:%d, line:%c%c\n", *i, line[*i], line[(*i)+1]);
	while (!is_space(line[*i]) && !is_meta(line[*i]) && line[*i])
		(*i)++;
	if (!(key = ft_substr(line, j, *i - j)))
		return (ft_error("[env_join] key malloc error", 1));
	len = ft_strlen(key);
	pos = *env;
	while (pos)
	{
		if (!(ft_strncmp(key, ((t_dict*)(pos->content))->key, len + 1)))
		{
			tmp = *new;
			if (!(*new = ft_strjoin(*new, ((t_dict *)(pos->content))->value)))
			{
				free(key);
				free(tmp);
				return (ft_error("[env_join] new malloc error", 1));
			}
			free(tmp);
			break;
		}
		pos = pos->next;
	}
	free(key);
	return (0);
}

static int pre_join(char **new, char *line, int i, int j)
{
	char	*tmp;
	char	*add;

	if (!(add = ft_substr(line, j, i - j)))
		return (ft_error("[pre_join] add malloc failed", 1));
	tmp = *new;
	if (!(*new = ft_strjoin(*new, add)))
	{
		free(tmp);
		free(add);
		return (ft_error("[pre_join] new malloc failed", 1));
	}
	free(tmp);
	free(add);
	return (0);
}

static	char *env_solve(char *line, t_list **env)
{
	char	*new;
	int		i;
	int		j;
	int 	quote_flag;

	new = ft_strdup("");
	i = -1;
	j = 0;
	quote_flag = 0;
	while (line[++i] && new)
	{
		if (line[i] == '\'' && quote_flag != 2)
			quote_flag ^= 1;
		if (line[i] == '"' && quote_flag != 1)
			quote_flag ^= 2;
		if (line[i] == '\\' && !quote_flag && ++i)
			continue;
		if (quote_flag || line[i] != '$')
			continue;
		if (pre_join(&new, line, i, j))
			return (NULL);
		if (env_join(&new, env, line, &i))
			return (NULL);
		j = i;		
	}
	if (j != i)
		if (pre_join(&new, line, i, j))
			return (NULL);
	return (new);
}

int expand_env(t_list **lines, t_list **env)
{
	t_list *mov;
	char *tmp;
	
	mov = *lines;
	while (mov)
	{
		tmp = (char*)mov->content;
		if (!(mov->content = env_solve(tmp, env)))
		{
			free(tmp);
			ft_lstclear(lines, free);
			return (1);
		}
		free(tmp);
		mov = mov->next;
	}
	return (0);
}
