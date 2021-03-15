/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/09 16:28:47 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/15 17:19:52 by syamashi         ###   ########.fr       */
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
# define MINISHELL "minishell: "
# define PROMPT_NAME ""
# define NOCURRENT 1
# define NXCURRENT 2

# define STDIN 0
# define STDOUT 1
# define STDERR 2

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
	int		fd_err;
	bool	error_flag;
}				t_exec;

typedef struct	s_leaf
{
	t_list	*str;
	t_list	*dir;
}				t_leaf;

typedef	struct	s_pack
{
	char	*line;
	int		type;
}				t_pack;

typedef struct	s_token
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

typedef struct	s_minishell
{
	t_list	*env_list;
	int		exit_status;
	char	*home_defvalue;
	t_list	*pwds;
	bool	pwd_dslash;
	char	*env_pwd;
	char	*env_oldpwd;
	t_list	*fd_backup;
}				t_minishell;

typedef struct s_command
{
	void				*content;
	int					and_or;
	struct s_command	*next;
}				t_command;

typedef struct s_redirect
{
	int rint;
	int backup;
}				t_redirect;

sig_atomic_t	g_intflag;

void	minishell(char **envp);
t_command	*div_commands(t_minishell *m_sh, char *line);
char	*sh_prompt(t_minishell *m_sh);
t_list	*to_ex_list(t_minishell *m_sh, t_list **pack_list);
int		sh_launch(t_minishell *m_sh, t_list *exlist);

void	del_command(void *pack_list_arg);
void	del_t_exec(void *exec_arg);

char	*key_get(char *line);
char	*value_get(char	*key, t_minishell *m_sh);
bool	key_find(char *key, t_minishell *m_sh);
char	*value_add(t_minishell *m_sh, char *key, char *value);
t_list	*pwdlst_init(char *str, int delflag);
char	*pwds_str(t_minishell *m_sh);
bool	pwdshell_exist(char *key, t_minishell *m_sh);
t_list	*pwdlst_nocurrent(char *str);
t_list	*pwdlst_solve(char *str);
t_list	*pwdlst_init(char *str, int delflag);
/*
int		sh_echo(char **argv);
int		sh_cd(char **argv);
int		sh_env(char **argv);
int		sh_export(char **argv);
int		sh_unset(char **argv);
*/
#endif
