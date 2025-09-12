#include "minishell.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>


int	count_cmds(const t_pipeline *p)
{
	const t_cmd	*c;
	int			n;

	n = 0;
	c = p->head;
	while (c)
	{
		n++;
		c = c->next;
	}
	return (n);
}

int	is_single_builtin(const t_pipeline *p)
{
    if (!p || !p->head || p->head->next || !p->head->argv)
        return (0);
    if (!is_builtin(p->head->argv[0]))
        return (0);
    if (equals_ignoring_sentinels(p->head->argv[0], "exit")
        && (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)))
        return (0);
    return (1);
}



int	open_next_pipe(int need, int fd[2])
{
	fd[0] = -1;
	fd[1] = -1;
	if (!need)
		return (0);
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

int	after_fork_parent(int i, int n, int prev_r, int fd1, int fd0)
{
	close_if(prev_r);
	close_if(fd1);
	if (i < n - 1)
		return (fd0);
	close_if(fd0);
	return (-1);
}



