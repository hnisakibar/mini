#include "minishell.h"

static void	restore_io(int sin, int sout, int serr)
{
	if (sin != -1)
		dup2(sin, STDIN_FILENO), close(sin);
	if (sout != -1)
		dup2(sout, STDOUT_FILENO), close(sout);
	if (serr != -1)
		dup2(serr, STDERR_FILENO), close(serr);
}

static int	is_exit_single_cmd(const t_pipeline *pl)
{
	const char	*name;

	if (!is_interactive() || !pl || !pl->head
		|| pl->head->next || !pl->head->argv || !pl->head->argv[0])
		return (0);
	name = pl->head->argv[0];
	while (name && *name == 0x01)
		name++;
    return (ft_strcmp(name, "exit") == 0);
}

static void	free_strv(char **v)
{
	int i;

	if (!v)
		return ;
	i = 0;
	while (v[i])
		free(v[i++]);
	free(v);
}

static int run_builtin_in_parent(char **argv, t_ctx *ctx)
{
    int rc;

    ctx->in_parent_exit = 1;
    rc = run_builtin(argv, ctx);
    ctx->in_parent_exit = 0;
    return rc;
}

static void	save_stdio(int *sin, int *sout, int *serr)
{
    *sin = dup(STDIN_FILENO);
    *sout = dup(STDOUT_FILENO);
    *serr = dup(STDERR_FILENO);
}

static int	prepare_parent_redirs(t_pipeline *pl, t_ctx *ctx,
                            int sin, int sout, int serr)
{
    ctx->pipeline = (t_pipeline *)pl;
    if (apply_redirs(pl->head, ctx->env, ctx->last_status) != 0)
    {
        ctx->pipeline = NULL;
        restore_io(sin, sout, serr);
        return (1);
    }
    return (0);
}

int	handle_parent_exit(t_pipeline *pl, t_ctx *ctx, char **envp)
{
	int		sin; 
	int 	sout; 
	int 	serr;
	char	**expanded;
	int		rc;

	if (!is_exit_single_cmd(pl))
		return (0);
	save_stdio(&sin, &sout, &serr);
    (void)envp;
    if (prepare_parent_redirs(pl, ctx, sin, sout, serr))
        return (1);
    expanded = expand_argv(pl->head->argv, ctx->env, ctx->last_status);
    rc = run_builtin_in_parent(expanded, ctx);
    ctx->pipeline = NULL;
	free_strv(expanded);
	restore_io(sin, sout, serr);
    ctx->last_status = rc;
    return (1);
}
