#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	run_line(const char *line, t_ctx *ctx, char **envp)
{
	t_pipeline	*pl;
	int			perr;
	int			status;

	pl = NULL;
    perr = parse_line(line, &pl);
    if (perr == 0 && pl && pl->head)
    {
        if (handle_parent_exit(pl, ctx, envp))
        {
            free_pipeline(pl);
            return (0);
        }
        status = execute_pipeline(pl, ctx, envp);
        ctx->last_status = status;
    }
    else if (perr != 0)
        ctx->last_status = perr;
    free_pipeline(pl);
    return (0);
}

int	main(int argc, char **argv, char **envp)
{
    char	*line;
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
	while (1)
	{
		line = read_one_line();
		if (!line)
			break ;
        if (find_unquoted_or(line) >= 0)
            run_or_chain(line, &ctx, envp);
        else
            run_line(line, &ctx, envp);
        free(line);
    }
    free_all(&ctx);
    return (ctx.last_status);
}

