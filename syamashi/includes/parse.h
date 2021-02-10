/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:39:20 by syamashi          #+#    #+#             */
/*   Updated: 2021/02/10 16:22:28 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "../includes/minishell.h"

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

void	all_free(char **line, t_list **store, t_list **ast, t_list **exlist);
int		env_init(char **envp, t_list **env);
t_list	*ft_strtoken(char *line);
int		input_check(t_list *store);
void	store_div(t_list **store);
void	env_expand(t_list **packs, t_list **env, int r);
void	packs_trim(t_list **packs);
void 	pack_free(void *ptr);
void 	packs_free(t_list **packs);
void 	env_free(void *ptr);
void	all_free(char **line, t_list **store, t_list **ast, t_list **exlist);
int		is_space(char c);
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
int		solve_exit(t_list *semi, t_list **packs, t_list **env);
int		get_exec(t_list **store, t_list **packs);
void	def_strtoken(t_token *t, t_list **list, char **line, t_pack **pack);
t_list	*packed(char *line);
int		input_check(t_list *store);
int		ft_error(char *str, int i);
int		ft_syntax_error(char *str, int i);
int		ft_avoid_error(char *str, int i);
int		ft_exit_error(char *str, int i);
t_list	*ft_strtoken(char *line);
void	token_semi(t_pack **pack, t_list **list, t_token *t);
void	token_doll(t_pack **pack, t_list **list, t_token *t);
int		is_envmeta(char c);
void	store_div(t_list **store);
int		is_dir(int n);
void	packs_trim(t_list **packs);
void	exlist_init(t_list *ast, t_list **exlist, t_list **env, int *r);
void	ast_init(t_list **ast, t_list **packs);
void	quote_del(t_list **packs);
void	strs_join(t_list **packs);
#endif
