#include "minishell.h"
#include "parser_int.h"

t_cmd   *cmd_new(void)
{
    t_cmd   *c;

    c = xcalloc(1, sizeof(*c));
    return (c);
}

void    attach_cmd(t_pipeline *pl, t_cmdpair *cp)
{
    if (cp->cur)
        return ;
    cp->cur = cmd_new();
    if (!pl->head)
        pl->head = cp->cur;
    else
        cp->last->next = cp->cur;
}
