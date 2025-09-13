/* 42 Norm: split helpers to keep functions short */
#include "minishell.h"

static int	in_dquotes_until(const char *w, size_t pos)
{
    size_t  j;
    int     inq;

    inq = 0;
    j = 0;
    while (j < pos)
    {
        if ((unsigned char)w[j] == 0x02)
            inq = !inq;
        if ((unsigned char)w[j] == 0x01)
            j++;
        j++;
    }
    return (inq);
}

static void	append_unquoted_split(const char *val, char **out, size_t *len)
{
    const char  *p;
    int          spaced;

    p = val;
    spaced = 0;
    while (*p)
    {
        if (ft_isspace((unsigned char)*p))
        {
            if (!spaced && *len > 0)
            { app_c(out, len, ' '); spaced = 1; }
            p++;
        }
        else
        {
            app_c(out, len, *p++);
            spaced = 0;
        }
    }
}

static char	*dup_key_n(const char *src, size_t n)
{
    char *k;

    k = (char *)safe_malloc(n + 1);
    ft_memcpy(k, src, n);
    k[n] = '\0';
    return (k);
}

size_t  varname_len(const char *s)
{
    size_t i;

    if ((unsigned char)s[0] == 0x01 || (unsigned char)s[0] == 0x02)
        return (0);
    if (!(s[0] == '_' || ft_isalpha((unsigned char)s[0])))
        return (0);
    i = 1;
    while ((unsigned char)s[i] != 0x01 && (unsigned char)s[i] != 0x02 &&
           (s[i] == '_' || ft_isalnum((unsigned char)s[i])))
        i++;
    return (i);
}

int expand_dollar(const char *word, size_t *i, char **out, size_t *len,
                  t_env *env, int last_status)
{
    size_t      klen;
    char       *key;
    const char *val;
    int         inq;

    if (word[*i + 1] == '\0')
        return (0);
    if (word[*i + 1] == '?')
    { push_status(out, len, last_status); *i += 2; return (1); }
    if (ft_isdigit((unsigned char)word[*i + 1]))
    { *i += 2; return (1); }
    klen = varname_len(&word[*i + 1]);
    if (klen == 0)
    { app_c(out, len, '$'); (*i)++; return (1); }
    key = dup_key_n(&word[*i + 1], klen);
    val = env_get(env, key);
    if (!val) val = ""; /* treat NULL as empty */
    inq = in_dquotes_until(word, *i);
    if (inq)
        app_str(out, len, val, ft_strlen(val));
    else
        append_unquoted_split(val, out, len);
    free(key);
    *i += 1 + klen;
    return (1);
}

void    app_home(char **out, size_t *len, t_env *env)
{
    const char *home;

    home = env_get(env, "HOME");
    if (home && *home)
        app_str(out, len, home, ft_strlen(home));
    else
        app_c(out, len, '~');
}

static int  handle_markers(const char *w, size_t *i,
                           char **out, size_t *len,
                           int *in_sq, int *in_dq)
{
    if ((unsigned char)w[*i] == 0x01)
    {
        *in_sq = !*in_sq;
        (*i)++;
        if (w[*i])
            app_c(out, len, w[(*i)++]);
        return (1);
    }
    if ((unsigned char)w[*i] == 0x02)
    { *in_dq = !*in_dq; (*i)++; return (1); }
    return (0);
}

static int  handle_tilde_start(const char *w, size_t *i,
                               char **out, size_t *len, t_env *env)
{
    if (*i == 0 && w[*i] == '~' && (w[*i + 1] == '\0' || w[*i + 1] == '/'))
    { app_home(out, len, env); (*i)++; return (1); }
    return (0);
}

char    *expand_word(const char *w, t_env *env, int last_status)
{
    char    *out;
    size_t   len;
    size_t   i;
    int      in_dq;
    int      in_sq;

    out = NULL; len = 0; i = 0; in_dq = 0; in_sq = 0;
    while (w[i])
    {
        if (handle_markers(w, &i, &out, &len, &in_sq, &in_dq))
            continue;
        if (handle_tilde_start(w, &i, &out, &len, env))
            continue;
        if (w[i] == '$' && expand_dollar(w, &i, &out, &len, env, last_status))
            continue;
        app_c(&out, &len, w[i++]);
    }
    return (out ? out : ft_strdup(""));
}

static int  advance_var_or_fail(const char *w, size_t *i)
{
    size_t k;

    if (w[*i] != '$')
        return (0);
    if (w[*i + 1] == '?')
    { *i += 2; return (1); }
    k = varname_len(&w[*i + 1]);
    if (k == 0)
        return (0);
    *i += 1 + k;
    return (1);
}

int     is_pure_var_expr(const char *w)
{
    size_t  i;
    int     had;
    int     in_dq;
    int     in_sq;

    i = 0; had = 0; in_dq = 0; in_sq = 0;
    while (w[i])
    {
        if ((unsigned char)w[i] == 0x01)
            in_sq = !in_sq;
        else if ((unsigned char)w[i] == 0x02)
            in_dq = !in_dq;
        else if (!in_sq)
        {
            if (!advance_var_or_fail(w, &i))
                return (0);
            had = 1;
            continue;
        }
        i++;
    }
    return (had);
}
