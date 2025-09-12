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

int	msh_builtin_unset(char **argv, t_env **env)
{
    int	i;
    int had_bad;

    had_bad = 0;
    i = 1;
    while (argv[i])
    {
        if (is_valid_key(argv[i]))
            env_unset(env, argv[i]);
        else
        {
            ft_dprintf1(STDERR_FILENO,
                "minishell: unset: `%s': not a valid identifier\n",
                argv[i]);
            had_bad = 1;
        }
        i++;
    }
    return (had_bad ? 1 : 0);
}

