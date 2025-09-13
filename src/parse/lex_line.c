#include "minishell.h"
#include "lexer_int.h"

static void skip_ws(const char **ps)
{
    const char *s;

    s = *ps;
    while (*s == ' ' || *s == '\t')
        s++;
    *ps = s;
}

void    lex_line(const char *s, t_tokarr *ta)
{
    ta->v = NULL;
    ta->n = 0;
    ta->cap = 0;
    while (*s)
    {
        skip_ws(&s);
        if (!*s)
            break ;
        if (*s == '#')
        {
            while (*s && *s != '\n')
                s++;
            break ;
        }
        if (*s != '|' && *s != '<' && *s != '>')
            s = read_word(s, ta);
        else
            s = emit_operator(s, ta);
    }
}

void    free_tokens(t_tokarr *ta)
{
    size_t  i;

    if (!ta || !ta->v)
        return ;
    i = 0;
    while (i < ta->n)
    {
        if (ta->v[i].type == T_WORD && ta->v[i].val)
            free(ta->v[i].val);
        i++;
    }
    free(ta->v);
    ta->v = NULL;
    ta->n = 0;
    ta->cap = 0;
}
