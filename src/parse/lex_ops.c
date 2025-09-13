#include "minishell.h"
#include "lexer_int.h"

void    tok_push(t_tokarr *ta, t_tok t)
{
    size_t  newcap;

    if (ta->n == ta->cap)
    {
        newcap = (ta->cap ? ta->cap * 2 : 16);
        t_tok *nv = (t_tok *)safe_malloc(newcap * sizeof(t_tok));
        if (ta->v && ta->n)
            ft_memcpy(nv, ta->v, ta->n * sizeof(t_tok));
        free(ta->v);
        ta->v = nv;
        ta->cap = newcap;
    }
    ta->v[ta->n++] = t;
}

static void push_op(t_toktype type, t_tokarr *ta)
{
    t_tok t;

    t.type = type;
    t.val = NULL;
    t.glued_right = 0;
    t.had_quote = 0;
    tok_push(ta, t);
}

const char *emit_operator(const char *s, t_tokarr *ta)
{
    if (*s == '>' && *(s + 1) == '|')
    {
        t_tok t;
        t.type = T_WORD;
        t.val = ft_strdup(">|");
        tok_push(ta, t);
        return s + 2;
    }
    if (*s == '|')
        return (push_op(T_PIPE, ta), s + 1);
    if (*s == '<')
    {
        if (*(s + 1) == '<')
            return (push_op(T_HEREDOC, ta), s + 2);
        return (push_op(T_LT, ta), s + 1);
    }
    if (*s == '>')
    {
        if (*(s + 1) == '>')
            return (push_op(T_DGT, ta), s + 2);
        return (push_op(T_GT, ta), s + 1);
    }
    return (s);
}
