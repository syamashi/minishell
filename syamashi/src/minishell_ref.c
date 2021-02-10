/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihsell_ref.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 15:52:36 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/08 17:17:55 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	minishell(char **envp)
{
	char	*line;
	t_list	*com_list;
	t_list	*ex_list;
	int		ret;

	env_init(envp);
	while (1)
	{
		if (!(line = sh_prompt()))
			continue ;
		ret = div_command(line, com_list);
		free(line);
		if (ret < 0)
			continue ;
		while (com_list)
		{
			ex_init(&ex_list);
			sh_launch(ex_list);
			ft_lstclaer(exlist);
			com_list = com_list->next;
		}
		ft_lstclaer(com_list);
	}
	sh_destroy();
}
