/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/09 16:28:47 by ewatanab          #+#    #+#             */
/*   Updated: 2021/01/31 18:16:05 by syamashi         ###   ########.fr       */
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

typedef struct	s_commands
{
	t_list	*com;
	t_list	*args;
	t_list	*input;
	t_list	*output;
}	t_commands;

typedef	struct	s_exec //pipe単位のデータ?
{
	char	**argv;//読み込みながら作る
	char	**envp;
	int		fd_in; //リダイレクトで数値変更
	int		fd_out;
	int		echo_option;
}				t_exec;

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

void
minishell(char **envp);
int		sh_exit();
int		sh_echo(char **argv);
int		sh_cd(char **argv);
int		sh_env(char **argv);
int		sh_export(char **argv);
int		sh_unset(char **argv);

void 	pack_free(void *ptr);
void 	packs_free(t_list **packs);
void 	env_free(void *ptr);
void	all_free(t_list **lst, t_list **env);
int		is_space(char c);
int		is_meta(char c);
int		new_pack(t_pack **pack);
int		pack_join(t_pack **pack, char *str, int len);
int		pack_add(t_pack **pack, t_list **list, int type);
int		token_pipe(t_pack **pack, t_list **list, t_token *t, int type);
int		token_and(t_pack **pack, t_list **list, t_token *t, int type);
int		token_redirect_l(t_pack **pack, t_list **list, t_token *t, int type);
int		token_redirect_r(t_pack **pack, t_list **list, t_token *t, int type);
int		token_escape(t_pack **pack, t_list **list, t_token *t);
int		token_squote(t_pack **pack, t_list **list, t_token *t);
int		token_dquote(t_pack **pack, t_list **list, t_token *t);
void	def_strtoken(t_token *t, t_list **list, char **line, t_pack **pack);
int		init_env(char **envp, t_list **env);
int		divide_semicolon(t_list **lines, char *line);
int		expand_env(t_list **store, t_list **env);

int solve_exit(t_list *semi, t_list **packs, t_list **env);

int get_exec(t_list **store, t_list **packs);
void exlist_free(void *ptr);
void def_strtoken(t_token *t, t_list **list, char **line, t_pack **pack);
int packed(t_list **packs, t_list **lines);
int meta_check(t_list **packs);
int	ft_error(char *str, int i);
int ft_syntax_error(char *str, int i);
int ft_avoid_error(char *str, int i);
int ft_exit_error(char *str, int i);
#endif
