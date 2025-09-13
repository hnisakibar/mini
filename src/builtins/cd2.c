#include "minishell.h"

int	arg_count(char **av)
{
	int	n;

	n = 0;
	while (av && av[n])
		n++;
	return (n);
}

const char	*get_env_or_report(t_env *env, const char *key, const char *errmsg)
{
	const char	*val;

	val = env_get(env, key);
    if (!val || !*val)
    {
        ft_putstr_fd(errmsg, STDERR_FILENO);
        return (NULL);
    }
	return (val);
}

const char	*target_path(char **av, t_env *env)
{
	int	ac;

	ac = arg_count(av);
	if (ac == 1)
		return (get_env_or_report(env, "HOME",
			"minishell: cd: HOME not set\n"));
	if (ac == 2 && ft_strcmp(av[1], "--") == 0)
		return (get_env_or_report(env, "HOME",
			"minishell: cd: HOME not set\n"));
	if (ac == 2 && ft_strcmp(av[1], "-") == 0)
		return (get_env_or_report(env, "OLDPWD",
			"minishell: cd: OLDPWD not set\n"));
	return (av[1]);
}

void	update_pwd_oldpwd(t_env **env, const char *oldpwd)
{
	char	cwd[PATH_MAX];

	if (oldpwd && *oldpwd)
		env_set(env, "OLDPWD", oldpwd, 1);
	if (getcwd(cwd, sizeof(cwd)))
		env_set(env, "PWD", cwd, 1);
}

int	too_many_args(char **argv)
{
    if (arg_count(argv) > 2)
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
        return (1);
    }
	return (0);
}
