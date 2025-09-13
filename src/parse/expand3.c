#include "minishell.h"

int has_ifs_space(const char *s)
{
    while (s && *s)
    {
        if (*s == ' ' || *s == '\t')
            return 1;
        s++;
    }
    return 0;
}

void ensure_out_cap(char ***pargv, int *pcap, int need)
{
    if (need <= *pcap)
        return;
    {
        int newcap = (*pcap > 0 ? *pcap * 2 : need + 8);
        if (newcap < need)
            newcap = need;
        char **newv = (char **)safe_malloc((size_t)newcap * sizeof(char *));
        if (*pargv && *pcap > 0)
            ft_memcpy(newv, *pargv, (size_t)(*pcap) * sizeof(char *));
        free(*pargv);
        *pargv = newv;
        *pcap = newcap;
    }
}

void process_field_split(const char *expanded, char ***out, int *m, int *cap)
{
    const char *p = expanded;
    while (*p)
    {
        while (*p == ' ' || *p == '\t')
            p++;
        if (!*p)
            break;
        const char *q = p;
        while (*q && *q != ' ' && *q != '\t')
            q++;
        {
            size_t len = (size_t)(q - p);
            char *field = (char *)safe_malloc(len + 1);
            ft_memcpy(field, p, len);
            field[len] = '\0';
            ensure_out_cap(out, cap, *m + 2);
            (*out)[(*m)++] = field;
        }
        p = q;
    }
}

static int has_unquoted_dollar(const char *s)
{
    size_t i;
    int    in_d;

    i = 0;
    in_d = 0;
    while (s && s[i])
    {
        unsigned char c = (unsigned char)s[i];
        if (c == 0x01)
        { i += (s[i + 1] ? 2 : 1); continue; }
        if (c == 0x02)
        { in_d = !in_d; i++; continue; }
        if (c == '$' && !in_d)
            return (1);
        i++;
    }
    return (0);
}

static int has_any_quote_marker(const char *s)
{
    const unsigned char *p = (const unsigned char *)s;
    while (p && *p)
    {
        if (*p == 0x01 || *p == 0x02)
            return 1;
        p++;
    }
    return 0;
}

void process_expanded_arg(char ***out, int *m, int *cap,
                          const char *original_arg, char *expanded)
{
    if (!has_any_quote_marker(original_arg) &&
        (is_pure_var_expr(original_arg) || has_unquoted_dollar(original_arg)) &&
        has_ifs_space(expanded))
    {
        process_field_split(expanded, out, m, cap);
        free(expanded);
    }
    else if (!has_any_quote_marker(original_arg) && expanded[0] == '\0' &&
             (is_pure_var_expr(original_arg) || has_unquoted_dollar(original_arg)))
    {
        /* pure/unquoted expansion that resulted in empty: drop argument */
        free(expanded);
    }
    else
    {
        ensure_out_cap(out, cap, *m + 2);
        (*out)[(*m)++] = expanded;
    }
}

char **expand_argv(char **argv, t_env *env, int last_status)
{
    int i = 0, m = 0, cap = 8;
    char **out = (char **)safe_malloc(sizeof(char *) * (size_t)cap);

    while (argv && argv[i])
    {
        char *expanded = expand_word(argv[i], env, last_status);
        process_expanded_arg(&out, &m, &cap, argv[i], expanded);
        i++;
    }
    ensure_out_cap(&out, &cap, m + 1);
    out[m] = NULL;
    return out;
}
