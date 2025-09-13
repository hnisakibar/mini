#include "minishell.h"
#include "exec_int.h"

static void restore_io(int si, int so, int se)
{
    if (si != -1)
    {
        dup2(si, STDIN_FILENO);
        close(si);
    }
    if (so != -1)
    {
        dup2(so, STDOUT_FILENO);
        close(so);
    }
    if (se != -1)
    {
        dup2(se, STDERR_FILENO);
        close(se);
    }
}

static void free_strv_local(char **v)
{
    int i;

    if (!v)
        return ;
    i = 0;
    while (v[i])
        free(v[i++]);
    free(v);
}

static int handle_reparse_if_needed(char **expanded, t_ctx *ctx,
                                    char **envp)
{
	if (needs_reparse(NULL, expanded))
	{
		char *re_line = join_words(expanded);
		t_pipeline *pl = NULL;
		int rc = parse_line(re_line, &pl);
		free(re_line);
		free_strv_local(expanded);
		if (rc != 0)
		{
			if (pl)
				free_pipeline(pl);
			restore_io(ctx->saved_stdin, ctx->saved_stdout, ctx->saved_stderr);
			return rc;
		}
		rc = execute_pipeline(pl, ctx, envp);
		free_pipeline(pl);
		restore_io(ctx->saved_stdin, ctx->saved_stdout, ctx->saved_stderr);
		return rc;
	}
	return -1;
}

static int apply_redirs_or_fail(const t_cmd *cmd, t_ctx *ctx, char **expanded)
{
    if (apply_redirs(cmd, ctx->env, ctx->last_status) != 0)
    {
        free_strv_local(expanded);
        restore_io(ctx->saved_stdin, ctx->saved_stdout, ctx->saved_stderr);
        return (1);
    }
    return (0);
}

int run_single_builtin(const t_cmd *cmd, t_ctx *ctx, char **envp)
{
	char **expanded;
	int   rc;

	ctx->saved_stdin = dup(STDIN_FILENO);
	ctx->saved_stdout = dup(STDOUT_FILENO);
	ctx->saved_stderr = dup(STDERR_FILENO);
	expanded = expand_argv(cmd->argv, ctx->env, ctx->last_status);
	    rc = handle_reparse_if_needed(expanded, ctx, envp);
	if (rc != -1)
		return (rc);
    if (apply_redirs_or_fail(cmd, ctx, expanded))
		return (1);
	ctx->in_parent_exit = 1;
	rc = run_builtin(expanded, ctx);
	ctx->in_parent_exit = 0;
	free_strv_local(expanded);
	restore_io(ctx->saved_stdin, ctx->saved_stdout, ctx->saved_stderr);
	return (rc);
}
