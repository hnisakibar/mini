#include "minishell.h"

void	print_exit_if_interactive(void)
{
    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
        ft_putstr_fd("exit\n", STDOUT_FILENO);
}

void	free_argv_inline(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	exit_now(int status, char **argv, t_ctx *ctx)
{
    if (ctx->in_parent_exit)
    {
        free_argv_inline(argv);
        free_all(ctx);
    }
    exit(status);
}

int	is_numeric_arg(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
    while (s[i])
    {
        if (!ft_isdigit((unsigned char)s[i]))
            return (0);
        i++;
    }
	return (1);
}

unsigned char	parse_code_or_die(const char *s, char **argv, t_ctx *ctx)
{
    char		*endp;
    long long	code;

	errno = 0;
	endp = NULL;
	code = strtoll(s, &endp, 10);
    if (errno == ERANGE || endp == s || (endp && *endp))
    {
        ft_dprintf1(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n", s);
        exit_now(2, argv, ctx);
    }
    return ((unsigned char)code);
}
