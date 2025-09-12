#include "minishell.h"
#include "lexer_int.h"

static const char *add_unclosed(const char *s, const char *p,
                                size_t *len, char **buf)
{
    size_t add;

    add = (size_t)(p - s);
    {
        char *nb = (char *)safe_malloc(*len + add + 1);
        if (*buf && *len)
            ft_memcpy(nb, *buf, *len);
        ft_memcpy(nb + *len, s, add);
        free(*buf);
        *buf = nb;
    }
    *len += add;
    (*buf)[*len] = '\0';
    return (p);
}

static const char *find_quote_end(const char *s, char q)
{
    const char *p;

    p = s + 1;
    while (*p && *p != q)
        p++;
    return (p);
}

static void append_single_quote(const char *s, const char *p,
                                size_t *len, char **buf)
{
    
    const char *q;
    size_t      add;

    add = (size_t)(p - (s + 1));
    if (add == 0)
        return ;
    
    {
        char *nb = (char *)safe_malloc(*len + (add * 2) + 1);
        if (*buf && *len)
            ft_memcpy(nb, *buf, *len);
        free(*buf);
        *buf = nb;
    }
    q = s + 1;
    while (q < p)
    {
        (*buf)[(*len)++] = '\1';
        (*buf)[(*len)++] = *q++;
    }
    (*buf)[*len] = '\0';
}

static void append_double_quote(const char *s, const char *p,
                                size_t *len, char **buf)
{
    /* Insert a boundary marker (0x02) before the double-quoted chunk
       so that variable-name scanning (e.g. $T"o") does not consume
       characters coming from the quoted segment. Unlike 0x01 (single-quote
       sentinel), this 0x02 marker should NOT force literal copying; the
       expander will just skip it and continue, allowing $ inside double
       quotes to still expand. */
    size_t add = (size_t)(p - (s + 1));
    {
        char *nb = (char *)safe_malloc(*len + add + 1 /*marker*/ + 1);
        if (*buf && *len)
            ft_memcpy(nb, *buf, *len);
        nb[*len] = '\2'; /* boundary marker */
        if (add)
            ft_memcpy(nb + *len + 1, s + 1, add);
        free(*buf);
        *buf = nb;
    }
    *len += 1 + add;
    (*buf)[*len] = '\0';
}

const char *read_quoted(const char *s, char q, size_t *len, char **buf)
{
    const char *p;

    p = find_quote_end(s, q);
    if (*p != q)
        return (add_unclosed(s, p, len, buf));
    if (q == '\'')
        append_single_quote(s, p, len, buf);
    else
        append_double_quote(s, p, len, buf);
    return (p + 1);
}

