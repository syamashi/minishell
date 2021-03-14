/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export2_no_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 17:12:47 by syamashi          #+#    #+#             */
/*   Updated: 2021/03/15 01:08:26 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/sh_launch.h"

static	void	prejoin(int *i, int *j, char *value, char **output)
{
	char	*add;
	char	*tmp;

	if (!(add = ft_substr(value, *j, *i - *j)))
		exit(ft_error("malloc failed", 1));
	tmp = *output;
	if (!(*output = ft_strjoin(*output, add)))
		exit(ft_error("malloc failed", 1));
	free(tmp);
	free(add);
	*j = *i;
}

static	char	*make_outputvalue(char *value)
{
	char	*output;
	int		i;
	int		j;

	if (!value)
		return (NULL);
	if (!(output = ft_strdup("=\"")))
		exit(ft_error("malloc failed", 1));
	i = -1;
	j = 0;
	while (value[++i])
	{
		if (is_esc(value[i]))
		{
			if (i > j)
				prejoin(&i, &j, value, &output);
			simplejoin(&output, "\\");
		}
	}
	if (i > j)
		simplejoin(&output, value + j);
	simplejoin(&output, "\"");
	return (output);
}

static	char	*make_output(t_list *env)
{
	char	*output;
	char	*tmp;
	char	*key;
	char	*value;

	key = ((t_dict *)env->content)->key;
	value = make_outputvalue(((t_dict *)env->content)->value);
	if (!(output = ft_strdup("")))
		exit(ft_error("malloc failed", 1));
	simplejoin(&output, "declare -x ");
	simplejoin(&output, key);
	if (value)
	{
		simplejoin(&output, value);
		free(value);
	}
	return (output);
}

t_list	*dictlst_cpy(t_list *env_list)
{
	t_list	*env;
	t_list	*new;
	t_list	*mov;
	t_dict	*dict;

	env = NULL;
	mov = env_list;
	while (mov)
	{
		if (!(dict = (t_dict *)malloc(sizeof(t_dict))))
			exit(ft_error("", 1));
		dict->key = ft_strdup(((t_dict *)mov->content)->key);
		dict->value = ft_strdup(((t_dict *)mov->content)->value);
		if (!(new = ft_lstnew(dict)))
			exit(ft_error("", 1));
		ft_lstadd_back(&env, new);
		mov = mov->next;
	}
	return (env);
}

int				display_export(t_minishell *m_sh, t_exec *ex)
{
	t_list	*env;
	t_list	*mov;
	char	*output;

	env = dictlst_cpy(m_sh->env_list);
	env = quick_sort_list(env);
	mov = env;
	while (mov)
	{
		output = make_output(mov);
		ft_putstr_fd(output, ex->fd_out);
		free(output);
		ft_putstr_fd("\n", ex->fd_out);
		mov = mov->next;
	}
	ft_lstclear(&env, env_free);
	return (0);
}
