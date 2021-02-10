/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/09 16:28:47 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/10 17:37:32 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include <errno.h>
# include <stdbool.h>
# include <fcntl.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "../libft/libft.h"

# define STR_OPT_LN "-n"
# define EM_TOO_MANY_ARG "too many arg"
# define MINISHELL "minishell :"

typedef enum	e_errno
{
	TOO_MANY_ARG,
	FEW_ARG
}	t_errno;

typedef	struct	s_exec
{
	char	**argv;
	char	**envp;
	int		fd_in;
	int		fd_out;
	bool	error_flag;
}				t_exec;

typedef struct s_leaf
{
	t_list	*str;
	t_list	*dir;
}				t_leaf;

typedef struct s_pack
{
	char	*line;
	int		type;
}				t_pack;

typedef struct s_token
{
	char	*line;
	int		i;
	int		j;
}				t_token;

typedef struct	s_dict
{
	char *key;
	char *value;
}				t_dict;

void	minishell(char **envp);
int		sh_prompt(char **line);
int		sh_launch(t_list *exlist);
int		sh_exit();
int		sh_echo(char **argv);
int		sh_cd(char **argv);
int		sh_env(char **argv);
int		sh_export(char **argv);
int		sh_unset(char **argv);
#endif
