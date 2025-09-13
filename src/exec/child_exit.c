#include "minishell.h"
#include "exec_int.h"

void    free_strv(char **v)
{
    int i;

    if (!v)
        return ;
    i = 0;
    while (v[i])
    {
        free(v[i]);
        i++;
    }
    free(v);
}

void    child_exit(int rc, char **argv)
{
    free_strv(argv);
    _exit(rc);
}
