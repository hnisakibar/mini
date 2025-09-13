#include "minishell.h"
#include <sys/wait.h>

int wait_all(pid_t *pids, int n, pid_t last_pid)
{
    int     i;
    int     st;
    int     last_status;
    pid_t   w;

    last_status = 0;
    i = 0;
    while (i < n)
    {
        st = 0;
        w = waitpid(pids[i], &st, 0);
        if (w == last_pid)
        {
            if (WIFEXITED(st))
                last_status = WEXITSTATUS(st);
            else if (WIFSIGNALED(st))
                last_status = 128 + WTERMSIG(st);
        }
        i++;
    }
    return (last_status);
}
