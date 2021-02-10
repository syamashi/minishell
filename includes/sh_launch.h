/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_lounch.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 15:02:28 by ewatanab          #+#    #+#             */
/*   Updated: 2021/02/07 20:30:04 by ewatanab         ###   ########.fr       */
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

# define PATH_MAX 255

/*
 * typedef	struct	s_exec //pipe単位のデータ?
 * {
 * 	char	**argv;//読み込みながら作る
 * 	char	**envp;
 * 	int		fd_in; //リダイレクトで数値変更
 * 	int		fd_out;
 * }				t_exec;
 */

typedef int (*t_builtin_f)(t_exec *);


int		sh_lounch(t_list *execlist);
int		ft_execvpe(const char *file, char *const *argv, char *const *envp);

t_builtin_f	builtin_table(t_exec *com);

/*
 * utiles
 */
int		sh_execvpes(t_exec *s);
int		ft_perror(char *string);
void	sh_dup_close(int old_fd, int new_fd);

#endif