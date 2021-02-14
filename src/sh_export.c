/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 12:01:52 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/14 15:17:13 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/sh_launch.h"

char	*make_outputvalue(char *value)
{
	char	*output;
	char	*tmp;
	char	*add;
	int		i;
	int		j;

	if (!value)
		return (NULL);
	if (!(output = ft_strdup("=\"")))
		exit(ft_error("", 1));
	i = -1;
	j = 0;
	while (value[++i])
	{
		if (is_esc(value[i]))
		{
			if (i > j)
			{
				add = ft_substr(value, j, i - j);
				tmp = output;
				output = ft_strjoin(output, add);
				free(tmp);
				free(add);
				j = i;
			}
			tmp = output;
			output = ft_strjoin(output, "\\");
			free(tmp);
		}
	}
	if (i > j)
	{
		tmp = output;
		output = ft_strjoin(output, value + j);
		free(tmp);
	}
	tmp = output;
	output = ft_strjoin(output, "\"");
	free(tmp);
	return (output);
}

char	*make_output(t_list *env)
{
	char	*output;
	char	*tmp;
	char	*key;
	char	*value;

	key = ((t_dict *)env->content)->key;
	value = make_outputvalue(((t_dict *)env->content)->value);
	output = ft_strdup("");
	tmp = output;
	output = ft_strjoin(output, "declare -x ");
	free(tmp);
	tmp = output;
	output = ft_strjoin(output, key);
	free(tmp);
	if (value)
	{
		tmp = output;
		output = ft_strjoin(output, value);
		free(tmp);
	}
	return (output);
}

int	display_export(t_minishell *m_sh, int n)
{
	t_list	*env;
	char	*output;

	env = m_sh->env_list;
	while (env)
	{
		output = make_output(env);
		ft_putstr_fd(output, 1);
		free(output);
		ft_putstr_fd("\n", 1);
		env = env->next;
	}
	return (n);
}

int	is_keyvalid(char *key)
{
	int i;

	if (ft_isdigit(*key))
		return (0);
	i = -1;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
	}
	return (1);
}

void	invalid_key(char *argv, t_minishell *m_sh)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(argv, 2);
	ft_putstr_fd("': not a valid identifier", 2);
	ft_putstr_fd("\n", 2);
	m_sh->exit_status = 1;
}

void	envlst_add(t_list **env, t_list *new)
{
	char	*newkey;
	t_list	*mov;
	t_list	*prev;

	mov = *env;
	prev = NULL;
	newkey = ((t_dict *)new->content)->key;
	while (mov)
	{
		if (ft_strcmp(newkey, ((t_dict *)(mov)->content)->key) < 0)
		{
			if (prev)
			{
				prev->next = new;
				new->next = mov;
			}
			else
				ft_lstadd_front(env, new);
			return;
		}
		prev = mov;
		mov = mov->next;
	}
	ft_lstadd_back(env, new);
}

void	export_envp(t_minishell *m_sh, char *key, char *value)
{
	t_list	*env;
	t_list	*new;
	t_dict	*dict;

	env = m_sh->env_list;
	if (!(dict = (t_dict *)malloc(sizeof(t_dict))))
		exit(ft_error("", 1));
	while (env)
	{
		if (!ft_strcmp(key, ((t_dict *)env->content)->key))
		{
			if (value)
			{
				free(((t_dict *)env->content)->value);
				((t_dict *)env->content)->value = value;
			}
			return ;
		}
		env = env->next;
	}
	dict->key = key;
	dict->value = value;
	new = ft_lstnew(dict);
	envlst_add(&m_sh->env_list, new);
	return;
}

int	sh_export(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;
	char	*key;
	char	*value;
	int		i;

	argv = exec->argv + 1;
	if (!*argv)
		return (display_export(m_sh, 0));
	while (*argv)
	{
		key = key_get(*argv);
		if (is_keyvalid(key))
		{
			i = -1;
			while (!is_keyend((*argv)[++i]))
				;
			if (!(*argv)[i])
				value = NULL;
			else
				if (!(value = ft_strdup(*argv + i + 1)))
					exit(ft_error("", 1));
//			printf("[sh_export] key:[%s], value[%s], i:%d\n", key, value, i);
			export_envp(m_sh, key, value);
		}
		else
			invalid_key(*argv, m_sh);
		argv++;
	}
	return (0);
}
