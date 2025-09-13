/* 42 Norm: break down helpers to keep functions short */
#include "minishell.h"
#include "lexer_int.h"

static int  is_breaker(char c)
{
    return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

static void buf_append_n(char **buf, size_t *len, const char *src, size_t n)
{
    char *nb;

    nb = (char *)safe_malloc(*len + n + 1);
    if (*buf && *len)
        ft_memcpy(nb, *buf, *len);
    if (n)
        ft_memcpy(nb + *len, src, n);
    *len += n;
    nb[*len] = '\0';
    free(*buf);
    *buf = nb;
}

static const char *consume_one(const char *s, size_t *len,
                               char **buf, int *touched)
{
    if (*s == '$' && *(s + 1) == '\"')
    { *touched = 1; return (read_quoted(s + 1, '\"', len, buf)); }
    if (*s == '$' && *(s + 1) == '\'')
    { *touched = 1; return (read_quoted(s + 1, '\'', len, buf)); }
    if (*s == '\'' || *s == '\"')
    { *touched = 1; return (read_quoted(s, *s, len, buf)); }
    if (*s == '\\' && *(s + 1) != '\0')
    {
        char two[2];
        two[0] = '\x01';
        two[1] = *(s + 1);
        buf_append_n(buf, len, two, 2);
        *touched = 1;
        return (s + 2);
    }
    buf_append_n(buf, len, s, 1);
    *touched = 1;
    return (s + 1);
}

static void push_word(char *buf, int had_quote, t_tokarr *ta)
{
    t_tok t;

    if (!buf)
        return ;
    t.type = T_WORD;
    t.val = buf;
    t.glued_right = 0;
    t.had_quote = had_quote;
    tok_push(ta, t);
}

static void ensure_empty_if_touched(int touched, size_t len, char **buf)
{
    if (touched && len == 0 && *buf == NULL)
    {
        *buf = (char *)safe_malloc(1);
        (*buf)[0] = '\0';
    }
}

static void push_and_mark_glued(const char *s, char *buf,
                                int saw_quote, t_tokarr *ta)
{
    int glued;

    glued = (*s == '|' || *s == '<' || *s == '>');
    push_word(buf, saw_quote, ta);
    if (glued)
        ta->v[ta->n - 1].glued_right = 1;
}

const char *read_word(const char *s, t_tokarr *ta)
{
    char    *buf;
    size_t  len;
    int     touched;
    int     saw_quote;

    buf = NULL; len = 0; touched = 0; saw_quote = 0;
    while (*s && !is_breaker(*s))
    {
        if (*s == '\'' || *s == '"' || (*s == '$' && (*(s + 1) == '\'' || *(s + 1) == '"')))
            saw_quote = 1;
        s = consume_one(s, &len, &buf, &touched);
    }
    ensure_empty_if_touched(touched, len, &buf);
    push_and_mark_glued(s, buf, saw_quote, ta);
    return (s);
}
