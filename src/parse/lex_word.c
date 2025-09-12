#include "minishell.h"
#include "lexer_int.h"

static int  is_breaker(char c)
{
    return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

static const char *consume_one(const char *s, size_t *len,
                               char **buf, int *touched)
{
    /* Handle i18n-style quotes: $\"...\" and $'...'
       In our shell we don't implement translation; treat as normal quotes
       but do not start a variable expansion on the leading '$'. */
    if (*s == '$' && *(s + 1) == '\"')
    {
        *touched = 1;
        return (read_quoted(s + 1, '\"', len, buf));
    }
    if (*s == '$' && *(s + 1) == '\'')
    {
        *touched = 1;
        return (read_quoted(s + 1, '\'', len, buf));
    }
    /* Regular quotes */
    if (*s == '\'' || *s == '\"')
    {
        *touched = 1;
        return (read_quoted(s, *s, len, buf));
    }
    /* Unquoted backslash escapes the next character (if any). Protect
       that character from special handling by prefixing 0x01 marker. */
    if (*s == '\\' && *(s + 1) != '\0')
    {
        char *nb = (char *)safe_malloc(*len + 2 + 1);
        if (*buf && *len)
            ft_memcpy(nb, *buf, *len);
        nb[(*len)++] = '\x01';
        nb[(*len)++] = *(s + 1);
        nb[*len] = '\0';
        free(*buf);
        *buf = nb;
        *touched = 1;
        return (s + 2);
    }
    {
        char *nb = (char *)safe_malloc(*len + 2);
        if (*buf && *len)
            ft_memcpy(nb, *buf, *len);
        nb[(*len)++] = *s;
        nb[*len] = '\0';
        free(*buf);
        *buf = nb;
    }
    *touched = 1;
    return (s + 1);
}

static void push_word(char *buf, t_tokarr *ta)
{
    t_tok t;

    if (!buf)
        return ;
    t.type = T_WORD;
    t.val = buf;
    tok_push(ta, t);
}

const char *read_word(const char *s, t_tokarr *ta)
{
    char    *buf;
    size_t  len;
    int     touched;

    buf = NULL;
    len = 0;
    touched = 0;
    while (*s && !is_breaker(*s))
        s = consume_one(s, &len, &buf, &touched);
    /* If we touched input but produced no chars, ensure empty string token.
       Avoid double-alloc when read_quoted already allocated an empty buffer. */
    if (touched && len == 0 && buf == NULL)
    {
        buf = (char *)safe_malloc(1);
        buf[0] = '\0';
    }
    push_word(buf, ta);
    return (s);
}

