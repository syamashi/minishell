/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 15:02:28 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/19 16:04:13 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_LOUNCH_H
# define SH_LOUNCH_H

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

# include "../includes/minishell.h"
# include "../includes/parse.h"

# define PATH_MAX 255
# define F_ECHO_OPT_N (1 << 0)

/*
 * typedef	struct	s_exec //pipe単位のデータ?
 * {
 * 	char	**argv;//読み込みながら作る
 * 	char	**envp;
 * 	int		fd_in; //リダイレクトで数値変更
 * 	int		fd_out;
 * }				t_exec;
 */

typedef int (*t_builtin_f)(t_minishell *, t_exec *);


int		sh_launch(t_minishell *m_sh, t_list *execlist);
int		sh_execvpe(const char *file, char *const *argv, char *const *envp, t_minishell *m_sh);


/*
 * utiles
 */
int		sh_execvpes(t_exec *s, t_minishell *m_sh);
int		ft_perror(char *string);
void	sh_dup_close(int old_fd, int new_fd);
t_builtin_f	builtin_table(t_exec *com);

int		display_export(t_minishell *m_sh, t_exec *ex);
void	export_envp(t_minishell *m_sh, char *key, char *value);
int		is_keyvalid(char *key);
void	invalid_key(char *argv, int *is_invalid, char **key);

char	*value_get(char *key, t_minishell *m_sh);

/*
 * builtin functions
 */
int		sh_echo(t_minishell *m_sh, t_exec *exec);
int		sh_export(t_minishell *m_sh, t_exec *exec);
int		sh_unset(t_minishell *m_sh, t_exec *exec);
int		sh_env(t_minishell *m_sh, t_exec *exec);
int		sh_cd(t_minishell *m_sh, t_exec *exec);
int		sh_pwd(t_minishell *m_sh, t_exec *exec);
int		sh_exit(t_minishell *m_sh, t_exec *exec);

#endif
