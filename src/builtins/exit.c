#include "minishell.h"

static int	is_nonumeric_arg(char **argv, t_ctx *ctx)
{
    if (!is_numeric_arg(argv[1]))
    {
        ft_dprintf1(STDERR_FILENO,
            "minishell: exit: %s: numeric argument required\n", argv[1]);
        if (ctx->in_parent_exit)
            exit_now(2, argv, ctx);
        return (1);
    }
    return (0);
}

int	msh_builtin_exit(char **argv, t_ctx *ctx)
{
    unsigned char	code;

    print_exit_if_interactive();
    if (!argv[1])
    {
        if (ctx->in_parent_exit)
            exit_now(ctx->last_status, argv, ctx);
        return (ctx->last_status);
    }
    if (is_nonumeric_arg(argv, ctx))
        return (2);
    if (argv[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return (1);
    }
    code = parse_code_or_die(argv[1], argv, ctx);
    if (ctx->in_parent_exit)
        exit_now(code, argv, ctx);
    return (code);
}
