# include "../includes/sh_launch.h"

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


int		sh_exit(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;
	int		len;
	int		ret;

	argv = exec->argv + 1;
	if (!*argv)
		exit(0);
	if (*(argv + 1))
		return (ft_error("minishell: exit: too many arguments", 1));
	if ((ret = get_exitnum(*argv)) > 255)
		exit(ft_exit_error(*argv, 255));
	exit(ret);
}
