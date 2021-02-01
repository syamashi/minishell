/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 02:04:32 by syamashi          #+#    #+#             */
/*   Updated: 2021/01/30 19:31:59 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/*
bash: exit: 12a: numeric argument required
c1r1s6% echo $?
255

bash: exit: too many arguments
bash-3.2$ echo $?
1
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamashi <syamashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 22:15:35 by syamashi          #+#    #+#             */
/*   Updated: 2020/06/28 19:39:50 by syamashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_exitnum(const char *nptr)
{
	char		*str;
	int			n;
	int			num;
	long long	m;

	str = (char *)nptr;
	while (*str == ' ' || *str == '\t' ||
	*str == '\n' || *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	n = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-')
		str++;
	m = 0;
	while (ft_isdigit(*str))
	{
		num = (*str++ - '0');
		if ((m >= 922337203685477580 && num > 7) || m >= 922337203685477581)
			return (4242);
		if ((m <= -922337203685477580 && num > 8) || m <= -922337203685477581)
			return (4242);
		m = m * 10 + n * num;
	}
	if (*str != '\0')
		return (4242);
	return (((m % 256) + 256) % 256);
}

// exitの出力は実行だった。
// parseでexitしてはいけない。
int	solve_exit(t_list *pack, t_list **packs, t_list **env)
{
	int	ret;
	int lsize;

//	printf("[solve_exit] toppack:%s\n", ((t_pack *)pack->content)->line);
	if (ft_strncmp(((t_pack *)pack->content)->line, "exit", 5))
		return (0);
	ft_putstr("exit\n");
	lsize = ft_lstsize(pack);
	if (lsize > 2)
		return (ft_error("bash: exit: too many arguments", 1));
	pack = pack->next;
	if (!pack)
	{
		all_free(packs, env);
		exit(0);
	}
	if ((ret = get_exitnum(((t_pack*)pack->content)->line)) > 255)
		{
			ret = ft_exit_error(((t_pack *)pack->content)->line, 255);
			all_free(packs, env);
			exit(ret);
		}
	all_free(packs, env);
	exit(ret);
}
