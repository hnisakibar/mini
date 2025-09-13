#include "minishell.h"

const char *skip_quote_sentinel(const char *s)
{
    while (s && *s && (unsigned char)*s == 0x01)
        s++;
    return (s);
}

int	equals_ignoring_sentinels(const char *s, const char *lit)
{
    if (!s || !lit)
        return (0);
    while (*s && *lit)
    {
        if ((unsigned char)*s == 0x01)
        {
            s++;
            continue;
        }
        if (*s != *lit)
            return (0);
        s++;
        lit++;
    }
    while (*s && (unsigned char)*s == 0x01)
        s++;
    return (*s == '\0' && *lit == '\0');
}
