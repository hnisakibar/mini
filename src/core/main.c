#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static int	process_pipeline(t_pipeline *pl, t_ctx *ctx, char **envp)
{
    int status;

    if (handle_parent_exit(pl, ctx, envp))
        return (0);
    status = execute_pipeline(pl, ctx, envp);
    ctx->last_status = status;
    return (0);
}

static int	handle_parse_error(int perr, t_pipeline *pl, t_ctx *ctx)
{
    ctx->last_status = perr;
    if (!is_interactive())
    {
        free_pipeline(pl);
        return (-1);
    }
    return (0);
}

int	run_line(const char *line, t_ctx *ctx, char **envp)
{
    t_pipeline	*pl;
    int			perr;

	pl = NULL;
    perr = parse_line(line, &pl);
    if (perr == 0 && pl && pl->head)
        (void)process_pipeline(pl, ctx, envp);
    else if (perr != 0)
        return (handle_parse_error(perr, pl, ctx));
    free_pipeline(pl);
    return (0);
}

static int	handle_line_once(char *line, t_ctx *ctx, char **envp)
{
    if (find_unquoted_semi(line) >= 0)
        return (run_seq_chain(line, ctx, envp));
    if (find_unquoted_or(line) >= 0)
        return (run_or_chain(line, ctx, envp));
    return (run_line(line, ctx, envp));
}

static void	repl(t_ctx *ctx, char **envp)
{
    char *line;
    while (1)
    {
        int r;

        line = read_one_line();
        if (!line)
            break ;
        r = handle_line_once(line, ctx, envp);
        if (r < 0)
        {
            free(line);
            break ;
        }
        free(line);
    }
}

int	main(int argc, char **argv, char **envp)
{
    t_ctx   ctx;

	(void)argc;
	(void)argv;
    prompt_signals();
    (void)!isatty(STDIN_FILENO);
    ctx.env = env_init(envp);
    env_normalize_startup(&ctx.env);
    ctx.last_status = 0;
    ctx.in_parent_exit = 0;
    ctx.pids = NULL;
    ctx.pids_n = 0;
    ctx.pipeline = NULL;
    repl(&ctx, envp);
    free_all(&ctx);
    return (ctx.last_status);
}

