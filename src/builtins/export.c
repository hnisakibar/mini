#include "minishell.h"

static void	set_pair(t_env **env, const char *k, size_t klen, const char *v)
{
	char	*key;
	size_t	i;

	key = (char *)malloc(klen + 1);
	if (!key)
		return ;
	i = 0;
	while (i < klen)
	{
		key[i] = k[i];
		i++;
	}
	key[klen] = '\0';
	env_set(env, key, v, 1);
	free(key);
}

static int	export_no_args(t_env *env)
{
    size_t          n;
    const t_env   **arr;

    n = env_len(env);
    arr = (const t_env **)malloc(n * sizeof(*arr));
    if (!arr)
        return (1);
    env_collect(env, arr);
    if (n > 1)
        env_sort(arr, n);
    env_print_decl(arr, n);
    free((void *)arr);
    return (0);
}

static int	handle_export_arg(const char *arg, t_env **env)
{
    const char  *eq;
    size_t       klen;

    eq = strchr(arg, '=');
    klen = (eq ? (size_t)(eq - arg) : ft_strlen(arg));
    if (!valid_key(arg, klen))
    {
        ft_dprintf1(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", arg);
        return (1);
    }
    if (eq)
        set_pair(env, arg, klen, eq + 1);
    else
        env_set(env, arg, NULL, 0);
    return (0);
}

int	msh_builtin_export(char **argv, t_env **env)
{
    int i;
    int had_bad;

    if (!argv[1])
        return (export_no_args(*env));
    had_bad = 0;
    i = 1;
    while (argv[i])
    {
        if (argv[i][0] == '-' && argv[i][1] && argv[i][1] != '-')
        {
            char opt = argv[i][1];
            ft_putstr_fd("minishell: export: -", STDERR_FILENO);
            write(STDERR_FILENO, &opt, 1);
            ft_putstr_fd(": invalid option\n", STDERR_FILENO);
            ft_putstr_fd("export: usage: export [-fn] [name[=value] ...] or export -p\n", STDERR_FILENO);
            return (2);
        }
        if (handle_export_arg(argv[i], env))
            had_bad = 1;
        i++;
    }
    return (had_bad ? 1 : 0);
}
