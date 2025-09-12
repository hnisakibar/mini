#include "minishell.h"
#include "exec_int.h"

static void restore_io(int si, int so, int se)
{
    if (si != -1)
        dup2(si, STDIN_FILENO), close(si);
    if (so != -1)
        dup2(so, STDOUT_FILENO), close(so);
    if (se != -1)
        dup2(se, STDERR_FILENO), close(se);
}

static int handle_reparse_if_needed(char **expanded, t_ctx *ctx, char **envp, int si, int so, int se)
{
	if (needs_reparse(NULL, expanded))
	{
		char *re_line = join_words(expanded);
		t_pipeline *pl = NULL;
		int rc = parse_line(re_line, &pl);
		free(re_line);
		for (int i = 0; expanded[i]; i++)
			free(expanded[i]);
		free(expanded);
		if (rc != 0)
		{
			if (pl)
				free_pipeline(pl);
			restore_io(si, so, se);
			return rc;
		}
			rc = execute_pipeline(pl, ctx, envp);
		free_pipeline(pl);
		restore_io(si, so, se);
		return rc;
	}
	return -1; // no reparse needed
}
int run_single_builtin(const t_cmd *cmd, t_ctx *ctx, char **envp)
{
	int sin = dup(STDIN_FILENO);
	int sout = dup(STDOUT_FILENO);
	int serr = dup(STDERR_FILENO);
	char **expanded = expand_argv(cmd->argv, ctx->env, ctx->last_status);

    int rc = handle_reparse_if_needed(expanded, ctx, envp, sin, sout, serr);
	if (rc != -1)
		return rc;

	if (apply_redirs(cmd) != 0)
	{
		for (int i = 0; expanded[i]; i++) free(expanded[i]);
		free(expanded);
		restore_io(sin, sout, serr);
		return 1;
	}
	ctx->in_parent_exit = 1;
	rc = run_builtin(expanded, ctx);
	ctx->in_parent_exit = 0;

	for (int i = 0; expanded[i]; i++) free(expanded[i]);
	free(expanded);
	restore_io(sin, sout, serr);
	return rc;
}
