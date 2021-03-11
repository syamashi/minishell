/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 17:48:27 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/11 23:15:14 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parse.h"
#include "../includes/sh_launch.h"

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
		exit(ft_error("minishell: malloc failed", 1));
	j = ++i;
	while ((*envp)[i])
		i++;
	if (!((*map)->value = ft_substr(*envp, j, i - j)))
		exit(ft_error("minishell: malloc failed", 1));
}

void			env_oldpwd_init(t_minishell *m_sh)
{
	char	*key;
	char	*value;

	if (!(key = ft_strdup("OLDPWD")))
		exit(ft_error("minishell: malloc failed", 1));
	value = NULL;
	export_envp(m_sh, key, value);
}

void		env_pwd_init(t_minishell *m_sh)
{
	char	*pwd;
	char	*key;

	errno = 0;
	if (!(pwd = getcwd(NULL, 0)) && errno)
		ft_putendl_fd(strerror(errno), 2);
	if (!(key = ft_strdup("PWD")))
		exit(ft_error("minishell: malloc failed", 1));
	export_envp(m_sh, key, pwd);
}

int	shlvl_atoi(const char *nptr)
{
	char		*str;
	int			n;
	int			num;
	long long	m;

	str = (char *)nptr;
	while (*str == ' ' || *str == '\t' ||
	*str == '\n' || *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	n = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-')
		str++;
	m = 0;
	while (ft_isdigit(*str))
	{
		num = (*str++ - '0');
		if ((m >= 922337203685477580 && num > 7) || m >= 922337203685477581)
			return (0);
		if ((m <= -922337203685477580 && num > 8) || m <= -922337203685477581)
			return (0);
		m = m * 10 + n * num;
	}
	return (*str ? 0 : m);
}

void		env_shlvl_init(t_minishell *m_sh)
{
	char	*key;
	char	*value;
	int		depth;

	if (!(key = ft_strdup("SHLVL")))
		exit(ft_error("minishell: malloc failed", 1));
	if (!(value = value_get(key, m_sh)))
		depth = 0;
	else
		depth = shlvl_atoi(value);
	free(value);
	depth++;
	if (depth > 1000)
		shlvl_error(depth);
	depth = (depth < 0) ? 0 : depth;
	depth = (depth > 1000) ? 1 : depth;
	if (depth == 1000)
		value = ft_strdup("");
	else
		value = ft_itoa(depth);
	if (!value)
		exit(ft_error("minishell: malloc failed", 1));
	export_envp(m_sh, key, value);
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
			exit(ft_error("minishell: malloc failed", 1));
		init_map(&map, &m_sh->env_list, envp);
		if (!(new = ft_lstnew(map)))
			exit(ft_error("minishell: malloc failed", 1));
		ft_lstadd_back(&m_sh->env_list, new);
		envp++;
	}
	env_oldpwd_init(m_sh);
	env_pwd_init(m_sh);
	env_shlvl_init(m_sh);
	return (0);
}
