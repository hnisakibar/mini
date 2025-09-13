#include "minishell.h"

static int	is_valid_key(const char *s)
{
    size_t	i;

    if (!s || !*s || !(s[0] == '_' || ft_isalpha((unsigned char)s[0])))
    {
        return (0);
    }
    i = 1;
    while (s[i])
    {
        if (!(s[i] == '_' || ft_isalnum((unsigned char)s[i])))
            return (0);
        i++;
    }
    return (1);
}

static int	unset_handle_option(const char *opt)
{
    if (opt[1] == '-' && opt[2] == '\0')
        return (1);
    if (opt[1])
    {
        if (opt[1] == '-')
            ft_putstr_fd("minishell: unset: --: invalid option\n", STDERR_FILENO);
        else
        {
            ft_putstr_fd("minishell: unset: -", STDERR_FILENO);
            write(STDERR_FILENO, &opt[1], 1);
            ft_putstr_fd(": invalid option\n", STDERR_FILENO);
        }
        ft_putstr_fd("unset: usage: unset [-fv] [-n] [name ...]\n", STDERR_FILENO);
        return (2);
    }
    return (0);
}

static void	unset_one(const char *name, t_env **env, int *pbad)
{
    if (is_valid_key(name))
        (void)env_unset(env, name);
    else
    {
        ft_dprintf1(STDERR_FILENO,
            "minishell: unset: `%s': not a valid identifier\n",
            name);
        *pbad = 1;
    }
}

int	msh_builtin_unset(char **argv, t_env **env)
{
    int	i;
    int 	had_bad;

    had_bad = 0;
    i = 1;
    while (argv[i])
    {
        if (argv[i][0] == '-')
        {
            int r = unset_handle_option(argv[i]);
            if (r == 2)
                return (2);
            if (r == 1)
            { i++; continue; }
        }
        unset_one(argv[i], env, &had_bad);
        i++;
    }
    return (had_bad ? 1 : 0);
}
