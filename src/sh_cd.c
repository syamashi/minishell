# include "../includes/sh_launch.h"

/*
** export_envp() required malloced key/value and freed
*/

int		pwd_update(t_minishell *m_sh)
{
	char	*value;
	char	*buf;
	char	*pwdval;

	buf = NULL;
	if (!(value = value_get("PWD", m_sh)))
		if (!(value = ft_strdup("")))
			exit(ft_error("minishell: malloc failed", 1));
	if (key_find("OLDPWD", m_sh))
		export_envp(m_sh, ft_strdup("OLDPWD"), value);
	else
		free(value);
	if (key_find("PWD", m_sh))
	{
		errno = 0;
		if (!(pwdval = getcwd(buf, 1024)))
			return (ft_error(strerror(errno), 1));
		export_envp(m_sh, ft_strdup("PWD"), pwdval);
	}
	return (0);
}

int		cd_no_args(t_minishell *m_sh)
{
	char	*home;

	if (!key_find("HOME", m_sh) || !(home = value_get("HOME", m_sh)))
		return (ft_error("minishell: cd: HOME not set", 1));
	if (chdir(home) == -1)
	{
		free(home);
		return (ft_cd_error(home, 1));
	}
	free(home);
	return (0);
}

void	tilde_join(char **argv, t_minishell *m_sh)
{
	char	*tmp;
	char	*home_value;

	if (ft_strncmp(*argv, "~/", 2))
		return ;
	if (!key_find("HOME", m_sh) || !(home_value = value_get("HOME", m_sh)))
		if (!(home_value = ft_strdup(m_sh->home_defvalue)))
			if (!(home_value = ft_strdup("")))
				exit(ft_error("minishell: malloc failed", 1));
	tmp = *argv;
	*argv = ft_strjoin(home_value, *argv + 1);
	free(tmp);
	tmp = NULL;
	free(home_value);
	home_value = NULL;
}

int		sh_cd(t_minishell *m_sh, t_exec *exec)
{
	char	**argv;
	char	*pwd;
	char	*buf;

	buf = NULL;
	errno = 0;
	argv = exec->argv + 1;
	if (!*argv)
		return (cd_no_args(m_sh));
	if (!**argv)
		return (pwd_update(m_sh));
	tilde_join(argv, m_sh);
	if (chdir(*argv))
		return (ft_cd_error(*argv, 1));
	return (pwd_update(m_sh));
}

