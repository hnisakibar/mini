#include "minishell.h"

int  is_meta_only(const char *s)
{
    int has_meta;

    if (!s || !*s)
        return (0);
    has_meta = 0;
    while (*s)
    {
        if (*s != '|' && *s != '<' && *s != '>')
            return (0);
        has_meta = 1;
        s++;
    }
    return (has_meta);
}

int  has_sentinel(const char *s)
{
    const unsigned char *p = (const unsigned char *)s;
    while (p && *p)
    {
        if (*p == 0x01)
            return (1);
        p++;
    }
    return (0);
}

int  has_dollar(const char *s)
{
    return (s && strchr(s, '$') != NULL);
}

int  needs_reparse(char **orig, char **argv)
{
    int i = 0;
    if (!argv || !orig)
        return (0);
    while (argv[i] && orig[i])
    {
        if ((is_meta_only(argv[i]) || ft_strcmp(argv[i], "<<") == 0 || ft_strcmp(argv[i], ">>") == 0)
            && !has_sentinel(orig[i]) && has_dollar(orig[i]))
            return (1);
        i++;
    }
    return (0);
}

char *join_words(char **argv)
{
    size_t len = 0; int i = 0; char *s; size_t off = 0;
    while (argv[i])
    { 
        len += ft_strlen(argv[i]); 
        if (argv[i+1]) 
            len += 1; 
        i++; 
    }
    s = (char *)safe_malloc(len + 1);
    i = 0;
    while (argv[i])
    {
        size_t n = ft_strlen(argv[i]);
        ft_memcpy(s + off, argv[i], n);
        off += n;
        if (argv[i+1]) s[off++] = ' ';
        i++;
    }
    s[off] = '\0';
    return (s);
}
