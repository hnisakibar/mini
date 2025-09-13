#include "minishell.h"

int	is_builtin(const char *cmd)
{
    if (!cmd)
        return (0);
    return (equals_ignoring_sentinels(cmd, "echo")
        || equals_ignoring_sentinels(cmd, "pwd")
        || equals_ignoring_sentinels(cmd, "cd")
        || equals_ignoring_sentinels(cmd, "export")
        || equals_ignoring_sentinels(cmd, "unset")
        || equals_ignoring_sentinels(cmd, "exit")
        || equals_ignoring_sentinels(cmd, ":"));
}

int	run_builtin(char **argv, t_ctx *ctx)
{
	if (!ctx)
		return (0);
	if (!argv || !argv[0])
		return (0);
	if (equals_ignoring_sentinels(argv[0], "echo"))
		return (msh_builtin_echo(argv));
	if (equals_ignoring_sentinels(argv[0], "pwd"))
		return (msh_builtin_pwd());
	if (equals_ignoring_sentinels(argv[0], "cd"))
		return (msh_builtin_cd(argv, &ctx->env));
	if (equals_ignoring_sentinels(argv[0], "export"))
		return (msh_builtin_export(argv, &ctx->env));
	if (equals_ignoring_sentinels(argv[0], "unset"))
		return (msh_builtin_unset(argv, &ctx->env));
    if (equals_ignoring_sentinels(argv[0], "exit"))
        return (msh_builtin_exit(argv, ctx));
    if (equals_ignoring_sentinels(argv[0], ":"))
        return (0);
    return (0);
}
