#include "minishell.h"

static void	print_arg(const char *s)
{
    const char *p = s;
    while (*p)
    {
        if ((unsigned char)*p != 0x01)
            write(STDOUT_FILENO, p, 1);
        p++;
    }
}

static int	is_n_flag(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	i = 2;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	msh_builtin_echo(char **argv)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 0;
	while (argv[i] && is_n_flag(argv[i]))
	{
		no_newline = 1;
		i++;
	}
	while (argv[i])
	{
		print_arg(argv[i]);
        if (argv[i + 1])
            write(STDOUT_FILENO, " ", 1);
		i++;
	}
    if (!no_newline)
        write(STDOUT_FILENO, "\n", 1);
    return (0);
}

