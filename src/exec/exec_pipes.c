#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

static int handle_fork_result(pid_t pid, int i, int n,
	int prev_r, int fd[2], pid_t *last_pid)
{
	if (pid < 0)
		return (close_if(prev_r), close_pair(fd[0], fd[1]), 1);
	if (i == n - 1)
		*last_pid = pid;
	return after_fork_parent(i, n, prev_r, fd[1], fd[0]);
}

static int spawn_all_children(const t_pipeline *p, t_ctx *ctx, char **envp,
	pid_t *pids, int n, pid_t *last_pid)
{
	int			i = 0;
	int			prev_r = -1;
	int			fd[2];
	const t_cmd	*c = p->head;

	*last_pid = -1;
	while (i < n)
	{
		if (open_next_pipe(i < n - 1, fd))
			return (close_if(prev_r), 1);
		pids[i] = fork();
		if (pids[i] == 0)
			child_exec(c, ctx, envp, prev_r, fd);
		prev_r = handle_fork_result(pids[i], i, n, prev_r, fd, last_pid);
		c = c->next;
		i++;
	}
	close_if(prev_r);
	return (0);
}

static int wait_and_finish(pid_t *pids, int n, pid_t last_pid)
{
	return wait_all(pids, n, last_pid);
}

static int run_if_single_builtin(const t_pipeline *p, t_ctx *ctx, char **envp)
{
	int rc = run_single_builtin(p->head, ctx, envp);
	ctx->pipeline = NULL;
	return rc;
}

int	execute_pipeline(const t_pipeline *p, t_ctx *ctx, char **envp)
{
	int		n;
	pid_t	*pids;
	pid_t	last_pid;
	int		status;

	if (!p || !p->head)
		return (0);
	ctx->pipeline = (t_pipeline *)p;
	if (is_single_builtin(p))
		return run_if_single_builtin(p, ctx, envp);
	n = count_cmds(p);
	pids = safe_malloc(sizeof(pid_t) * n);
	ctx->pids = pids;
	ctx->pids_n = n;
	if (spawn_all_children(p, ctx, envp, pids, n, &last_pid))
		return (free(pids), ctx->pids = NULL, ctx->pids_n = 0,
				ctx->pipeline = NULL, 1);
	status = wait_and_finish(pids, n, last_pid);
	free(pids);
	ctx->pids = NULL;
	ctx->pids_n = 0;
	ctx->pipeline = NULL;
	return (status);
}
