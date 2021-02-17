/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 17:48:27 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/17 18:55:40 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

static	void	init_map(t_dict **map, t_list **env, char **envp)
{
	int i;
	int j;

	i = 0;
	(*map)->key = NULL;
	(*map)->value = NULL;
	while ((*envp)[i] != '=' && (*envp)[i])
		i++;
	if (!((*map)->key = ft_substr(*envp, 0, i)))
		exit(ft_error("", 1));
	j = ++i;
	while ((*envp)[i])
		i++;
	if (!((*map)->value = ft_substr(*envp, j, i - j)))
		exit(ft_error("", 1));
}

void			env_oldpwd_init(t_list **env)
{
	t_dict	*map;
	t_list	*new;
	int		i;
	int		j;

	map = NULL;
	new = NULL;
	if (!(map = (t_dict *)malloc(sizeof(t_dict))))
		exit(ft_error("minishell: malloc failed", 1));
	if (!(map->key = ft_strdup("OLDPWD")))
		exit(ft_error("minishell: malloc failed", 1));
	map->value = NULL;
	if (!(new = ft_lstnew(map)))
		exit(ft_error("minishell: malloc failed", 1));
	ft_lstadd_back(env, new);
}

int		env_init(char **envp, t_list **env)
{
	t_dict	*map;
	t_list	*new;
	int		i;
	int		j;

	map = NULL;
	new = NULL;
	while (*envp)
	{
		i = 0;
		if (!(map = (t_dict *)malloc(sizeof(t_dict))))
			exit(ft_error("", 1));
		init_map(&map, env, envp);
		if (!(new = ft_lstnew(map)))
			exit(ft_error("", 1));
		ft_lstadd_back(env, new);
		envp++;
	}
	env_oldpwd_init(env);
	return (0);
}
