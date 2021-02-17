/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:39:20 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/17 21:34:14 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "../includes/minishell.h"
# include "../includes/debug.h"

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
# define DUMMY 129

void	pack_free(void *ptr);
void	packs_free(t_list **packs);
void	env_free(void *ptr);
void	all_free(char **line, t_list **store, t_list **ast, t_list **exlist);
void	ast_free(t_list **ast);

int		env_init(char **envp, t_list **env);
t_list	*ft_strtoken(char *line);
void	store_div(t_command **store);
void	env_expand(t_list **packs, t_minishell *m_sh, int pathflag);
void	packs_trim(t_list **packs);
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
int		divide_semicolon(t_list **lines, char *line);
int		solve_exit(t_list *semi, t_list **packs, t_list **env);
int		get_exec(t_list **store, t_list **packs);
void	def_strtoken(t_token *t, t_list **list, char **line, t_pack **pack);
t_list	*packed(char *line);

int		syntax_check(t_list *list, t_minishell *m_sh);
int		input_check(t_list *store, t_minishell *m_sh);
void	token_semi(t_pack **pack, t_list **list, t_token *t);
void	token_doll(t_pack **pack, t_list **list, t_token *t);
void	packs_trim(t_list **packs);
void	exlist_init(t_list *ast, t_list **exlist, t_minishell *m_sh);
void	ast_init(t_list **ast, t_list **packs);
void	quote_del(t_list **packs);
void	strs_join(t_list **packs);

void	pack_stradd(t_pack **pack, t_list **list, t_token *t);
void	pack_metaadd(t_pack **pack, t_list **list, char *str, int type);
t_list	*pack_end(t_pack **pack, t_list **list);

void	env_solve(char **line, t_list *mov, t_minishell *m_sh);
void	repack(t_list **prev, t_list **mov, t_list **packs);
void	env_join(char **new, t_list *mov, t_token *t, t_minishell *m_sh);

void	pack_del(t_list **prev, t_list **mov, t_list **packs);

void	fd_controller(t_exec **ex, t_list *dir, t_minishell *m_sh);
void	fdin_set(t_exec **ex, const int n, char *path);
void	fdout_set(t_exec **ex, const int n, char *path);
void	quoteflag_get(int type, int *quote_flag);
char	*path_make(char *src, t_minishell *m_sh);

bool	is_space(char c);
bool	is_meta(char c);
bool	is_keyend(char c);
bool	is_envmeta(char c);
bool	is_dir(int n);
bool	is_quote(int n);
bool	is_dir(int n);
bool	is_metatype(int n);
bool	is_bonus(int n);
bool	isnot_cmd(const int type);
bool	is_cmd(int type);
bool	is_cmdstr(int type, int *pre_type, int *quote_flag);
bool	is_esc(char c);

void	packinfo_get(char **line, int *type, const t_list *mov);
void	simplejoin(char **output, char *add);
char	*simpletrim(char *output, char *trim);

t_command	*ft_clstnew(void *content);
void	ft_clstdelone(t_command *lst, void (*del)(void*));
void	ft_clstadd_back(t_command **lst, t_command *new);

bool	ambiguous_error(t_minishell *m_sh, char *str, t_exec **ex);
int		ft_syntax_error(char *str, int i);
int		ft_avoid_error(char *str, int i);
int		ft_exit_error(char *str, int i);
int		ft_error(char *str, int i);
void	fd_error(char *str, int fd);
int		dir_error(char *path, int n);
int		ft_cd_error(char *path, int n);
t_list	*quick_sort_list(t_list *c);
#endif
