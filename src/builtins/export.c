#include "minishell.h"

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
    const char	*eq;
    size_t		klen;
    int			append;

    eq = ft_strchr(arg, '=');
    append = 0;
    klen = (eq ? (size_t)(eq - arg) : ft_strlen(arg));
    if (eq && klen > 0 && arg[klen - 1] == '+')
    {
        append = 1;
        klen -= 1;
    }
    if (!valid_key(arg, klen))
    {
        ft_dprintf1(STDERR_FILENO,
            "minishell: export: `%s': not a valid identifier\n", arg);
        return (1);
    }
    if (eq)
        return export_assign(env, arg, klen, append, eq + 1);
    env_set(env, arg, NULL, 0);
    return (0);
}

int	msh_builtin_export(char **argv, t_env **env)
{
    int	i;
    int	had_bad;

    if (!argv[1])
        return (export_no_args(*env));
    had_bad = 0;
    i = 1;
    while (argv[i])
    {
        if (argv[i][0] == '-')
        {
            int r = export_handle_option(argv[i]);
            if (r == 2)
                return (2);
            if (r == 1)
            { i++; continue; }
        }
        if (handle_export_arg(argv[i], env))
            had_bad = 1;
        i++;
    }
    return (had_bad ? 1 : 0);
}
