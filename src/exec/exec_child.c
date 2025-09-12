#include "minishell.h"
#include <signal.h>
#include <unistd.h>

void	close_pair(int fd0, int fd1)
{
	if (fd0 != -1)
		close(fd0);
	if (fd1 != -1)
		close(fd1);
}

void	close_if(int fd)
{
	if (fd != -1)
		close(fd);
}

static int has_stdin_redir(const t_cmd *c)
{
    const t_redir *r = c ? c->redirs : NULL;
    while (r)
    {
        if (r->type == R_IN || r->type == R_HEREDOC)
            return 1;
        r = r->next;
    }
    return 0;
}
static int has_stdout_redir(const t_cmd *c)
{
    const t_redir *r = c ? c->redirs : NULL;
    while (r)
    {
        if (r->type == R_OUT_TRUNC || r->type == R_OUT_APPEND)
            return 1;
        r = r->next;
    }
    return 0;
}

void	child_exec(const t_cmd *c, t_ctx *ctx, char **envp, int prev_r, int fd[2])
{
	int		rc;
	char	**expanded;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	/* Apply pipe endpoints only if not overridden by redirections */
	if (prev_r != -1 && !has_stdin_redir(c) && dup2(prev_r, STDIN_FILENO) == -1)
		_exit(1);
	if (fd && fd[1] != -1 && !has_stdout_redir(c) && dup2(fd[1], STDOUT_FILENO) == -1)
		_exit(1);
	close_if(prev_r);
	if (fd)
		close_pair(fd[0], fd[1]);
	rc = apply_redirs(c);
	if (rc != 0)
		_exit(rc);
    expanded = expand_argv(c->argv, ctx->env, ctx->last_status);
    exec_inplace(c->argv, expanded, ctx, envp);
	_exit(127);
}

