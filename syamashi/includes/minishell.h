/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/09 16:28:47 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/10 15:21:46 by syamashi         ###   ########.fr       */
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

# define STR 0
# define RDIR 1
# define RRDIR 2
# define LDIR 3
# define LLDIR 4
# define LLLDIR 5
# define SPACE 6
# define SQUOTE 7
# define DQUOTE 8
# define PIPE 9
# define DPIPE 10
# define AND 11
# define DAND 12
# define SCOLON 13
# define DOLL 14
# define ESC 15
# define SSTR 16

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
void all_free(char **line, t_list **store, t_list **ast, t_list **exlist);
int is_space(char c);
int		is_meta(char c);
void	new_pack(t_pack **pack);
void	pack_join(t_pack **pack, char *str, int len);
void	pack_add(t_pack **pack, t_list **list, int type);
void	token_pipe(t_pack **pack, t_list **list, t_token *t);
void	token_and(t_pack **pack, t_list **list, t_token *t);
void	token_redirect_l(t_pack **pack, t_list **list, t_token *t);
void	token_redirect_r(t_pack **pack, t_list **list, t_token *t);
void	token_escape(t_pack **pack, t_list **list, t_token *t);
void	token_squote(t_pack **pack, t_list **list, t_token *t);
void	token_dquote(t_pack **pack, t_list **list, t_token *t);
void	def_strtoken(t_token *t, t_list **list, char **line, t_pack **pack);
int		env_init(char **envp, t_list **env);
int		divide_semicolon(t_list **lines, char *line);
void	env_expand(t_list **packs, t_list **env, int r);

int solve_exit(t_list *semi, t_list **packs, t_list **env);

int get_exec(t_list **store, t_list **packs);
void def_strtoken(t_token *t, t_list **list, char **line, t_pack **pack);
t_list	*packed(char *line);
int	input_check(t_list *store);
int ft_error(char *str, int i);
int ft_syntax_error(char *str, int i);
int ft_avoid_error(char *str, int i);
int ft_exit_error(char *str, int i);
t_list *ft_strtoken(char *line);
void token_semi(t_pack **pack, t_list **list, t_token *t);
void token_doll(t_pack **pack, t_list **list, t_token *t);
int is_envmeta(char c);
void store_div(t_list **store);
int is_dir(int n);
void	packs_trim(t_list **packs);
void exlist_init(t_list *ast, t_list **exlist, t_list **env, int *r);
void ast_init(t_list **ast, t_list **packs);
void	quote_del(t_list **packs);
void	strs_join(t_list **packs);
void exlist_debug(t_list *exlist);
#endif
