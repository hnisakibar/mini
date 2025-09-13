#include "minishell.h"

static int	chdir_report(const char *path)
{
    if (chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(strerror(errno), STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        return (1);
    }
	return (0);
}

static void	print_cwd_if_dash(char **argv)
{
	char	cwd[PATH_MAX];

	if (arg_count(argv) == 2 && ft_strcmp(argv[1], "-") == 0)
	{
		if (getcwd(cwd, sizeof(cwd)))
			printf("%s\n", cwd);
	}
}

int	msh_builtin_cd(char **argv, t_env **env)
{
	char		oldpwd[PATH_MAX];
	const char	*path;

	if (too_many_args(argv))
		return (1);
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	path = target_path(argv, *env);
	if (!path)
		return (1);
	if (chdir_report(path))
		return (1);
	update_pwd_oldpwd(env, oldpwd);
	print_cwd_if_dash(argv);
    return (0);
}
