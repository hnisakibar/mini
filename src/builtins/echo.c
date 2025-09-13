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
    const unsigned char *p;
    int  saw_n;

    if (!s)
        return (0);
    p = (const unsigned char *)s;
    while (*p == 0x01 || *p == 0x02)
        p++;
    if (*p != '-')
        return (0);
    p++;
    saw_n = 0;
    while (*p)
    {
        if (*p == 0x01 || *p == 0x02)
        { p++; continue; }
        if (*p != 'n')
            return (0);
        saw_n = 1;
        p++;
    }
    return (saw_n);
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
