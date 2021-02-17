/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_export2_no_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 17:12:47 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/17 18:29:50 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh_launch.h"

static	void	prejoin(int *i, int *j, char *value, char **output)
{
	char	*add;
	char	*tmp;

	if (!(add = ft_substr(value, *j, *i - *j)))
		exit(ft_error("minishell: malloc failed", 1));
	tmp = *output;
	if (!(*output = ft_strjoin(*output, add)))
		exit(ft_error("minishell: malloc failed", 1));
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
		exit(ft_error("minishell: malloc failed", 1));
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
		exit(ft_error("minishell: malloc failed", 1));
	simplejoin(&output, "declare -x ");
	simplejoin(&output, key);
	if (value)
	{
		simplejoin(&output, value);
		free(value);
	}
	return (output);
}

int				display_export(t_minishell *m_sh, t_exec *ex)
{
	t_list	*env;
	char	*output;

	env = m_sh->env_list;
	while (env)
	{
		output = make_output(env);
		ft_putstr_fd(output, ex->fd_out);
		free(output);
		ft_putstr_fd("\n", ex->fd_out);
		env = env->next;
	}
	return (0);
}
