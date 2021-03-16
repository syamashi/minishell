/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 17:48:27 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/16 21:26:12 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"
#include "../includes/sh_launch.h"

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
		exit(ft_error("malloc failed", 1, STDERR));
	j = ++i;
	while ((*envp)[i])
		i++;
	if (!((*map)->value = ft_substr(*envp, j, i - j)))
		exit(ft_error("malloc failed", 1, STDERR));
}

void		env_oldpwd_init(t_minishell *m_sh)
{
	char	*key;
	char	*value;

	if (!(key = ft_strdup("OLDPWD")))
		exit(ft_error("malloc failed", 1, STDERR));
	value = NULL;
	export_envp(m_sh, key, value);
}

void		env_pwd_init(t_minishell *m_sh)
{
	char	*pwd;
	char	*key;

	errno = 0;
	if (!(pwd = getcwd(NULL, 0)) && errno)
		ft_putendl_fd(strerror(errno), STDERR);
	if (!(key = ft_strdup("PWD")))
		exit(ft_error("malloc failed", 1, STDERR));
	export_envp(m_sh, key, pwd);
}

int			env_init(char **envp, t_minishell *m_sh)
{
	t_dict	*map;
	t_list	*new;

	map = NULL;
	new = NULL;
	while (*envp)
	{
		if (!(map = (t_dict *)malloc(sizeof(t_dict))))
			exit(ft_error("malloc failed", 1, STDERR));
		init_map(&map, &m_sh->env_list, envp);
		if (!(new = ft_lstnew(map)))
			exit(ft_error("malloc failed", 1, STDERR));
		ft_lstadd_back(&m_sh->env_list, new);
		envp++;
	}
	env_oldpwd_init(m_sh);
	env_pwd_init(m_sh);
	env_shlvl_init(m_sh);
	return (0);
}
