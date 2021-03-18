/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 15:02:28 by ewatanab          #+#    #+#             */
/*   Updated: 2021/03/18 15:50:10 by ewatanab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_LAUNCH_H
# define SH_LAUNCH_H

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
# define F_ECHO_OPT_N 1
# define MYENOENT "No such file or directory"

typedef int		(*t_builtin_f)(t_minishell *, t_exec *);

int				sh_launch(t_minishell *m_sh, t_list *execlist);
void			search_and_exec(char *env_path, char *const *argv,
				char *const *envp);

/*
**  utiles
*/

void			simple_case(const char *file, char *const *argv,
				char *const *envp);
bool			if_executable(int mode);
void			exec_command(t_minishell *m_sh, t_exec *exec_param);
int				ft_perror(char *string, int fd_err);
void			sh_dup_close(int old_fd, int new_fd, int fd_err);
t_builtin_f		builtin_table(t_exec *com);

int				display_export(t_minishell *m_sh, t_exec *ex);
void			export_envp(t_minishell *m_sh, char *key, char *value);
int				is_keyvalid(char *key);
int				invalid_key(char *argv, char **key, char *command, int fd);

char			*value_get(char *key, t_minishell *m_sh);

void			lstlast_del(t_minishell *m_sh);
void			pwdlst_cpy(t_minishell *sh, t_minishell *m_sh);
void			pwdlst_update(t_minishell *m_sh, char *argv, int delflag);
void			pwdlst_add(t_minishell *m_sh, char *str);
char			*pwds_str(t_minishell *m_sh);
char			*pwds_joinfree(char *pwd, char *add);
int				pwd_update(t_minishell *m_sh, char *argv, int delflag);
bool			is_linkdel(t_minishell *m_sh, char *argv);
t_list			*pwdlst_init(char *str, int delflag);

/*
**  builtin functions
*/

int				sh_echo(t_minishell *m_sh, t_exec *exec);
int				sh_export(t_minishell *m_sh, t_exec *exec);
int				sh_unset(t_minishell *m_sh, t_exec *exec);
int				sh_env(t_minishell *m_sh, t_exec *exec);
int				sh_cd(t_minishell *m_sh, t_exec *exec);
int				sh_pwd(t_minishell *m_sh, t_exec *exec);
int				sh_exit(t_minishell *m_sh, t_exec *exec);

#endif
