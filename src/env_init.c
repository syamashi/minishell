/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 17:48:27 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/16 17:15:35 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"

static void	env_error(char *str, t_dict **map, t_list **env)
{
	ft_error(str, 1);
	if (map)
	{
		free((*map)->key);
		(*map)->key = NULL;
		free((*map)->value);
		(*map)->value = NULL;
	}
	if (env)
		ft_lstclear(env, env_free);
	exit(1);
}

static void	init_map(t_dict **map, t_list **env, char **envp)
{
	int i;
	int j;

	i = 0;
	(*map)->key = NULL;
	(*map)->value = NULL;
	while ((*envp)[i] != '=' && (*envp)[i])
		i++;
	if (!((*map)->key = ft_substr(*envp, 0, i)))
		env_error("[init_env] (*map)->key malloc failed", map, env);
	j = ++i;
	while ((*envp)[i])
		i++;
	if (!((*map)->value = ft_substr(*envp, j, i - j)))
		env_error("[init_env] (*map)->value malloc failed", map, env);
}

void	env_oldpwd_init(t_list **env)
{
	t_dict	*map;
	t_list	*new;
	int		i;
	int		j;

	map = NULL;
	new = NULL;
	if(!(map = (t_dict *)malloc(sizeof(t_dict))))
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
			env_error("[init_env] map malloc failed", NULL, env);
		init_map(&map, env, envp);
		if (!(new = ft_lstnew(map)))
			env_error("[init_env] new malloc failed", &map, env);
		ft_lstadd_back(env, new);
		envp++;
	}
	env_oldpwd_init(env);
	return (0);
}
