#include "minishell.h"

void	*lxr_xrealloc(void *ptr, size_t n)
{
    void    *q;

    if (n == 0)
        n = 1;
    q = realloc(ptr, n);
    if (!q)
    {
        perror("minishell: realloc");
        exit(EXIT_FAILURE);
    }
    return (q);
}

