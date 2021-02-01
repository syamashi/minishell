/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 17:44:13 by ewatanab          #+#    #+#             */
/*   Updated: 2020/09/25 11:33:28 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	t_list	*g_env;

void	env_init(char **envp)
{
	if (g_env)
		ft_lstclear(&g_env, free);
	while (*envp)
		ft_lstadd_back(&g_env, ft_lstnew(ft_strdup((*envp)++)));
}

void	env_destroy()
{
	ft_lstclear(&g_env, free);
}

char	*ft_getenv(const char *name)
{
	char	*key;
	t_list	*tmp;

	if (!g_env)
		return (NULL);
	key = ft_strjoin(name, "=");
	tmp = ft_lstfindb(g_env, start_with, key);
	free(key);
	if (tmp)
		return(ft_strchr(tmp->content, '=') + 1);
	return (NULL);
}

int		ft_setenv(const char *name, const char *value, int overwrite)
{
	char	*key;
	t_list	*tmp;

	key = ft_strjoin(name, "=");
	tmp = ft_lstfindb(g_env, start_with, key);
	free(key);
	if (!tmp)
		ft_lstadd_back(&g_env, ft_lstnew(ft_strjoin(key, value)));
	else if (overwrite)
	{
		free(tmp->content);
		tmp->content = ft_strjoin(key, value);
	}
	free(key);
	return (0);
}

void	ft_unsetenv(const char *name)
{
	char	*key;

	key = ft_strjoin(name, "=");
	ft_lstdelifb(&g_env, free, start_with, key);
	free(key);
}
