#include "minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

int	ft_strcmp(const char *a, const char *b)
{
    size_t i = 0;
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    while (a[i] && b[i] && a[i] == b[i])
        i++;
    return ((unsigned char)a[i] - (unsigned char)b[i]);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dup;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = (char *)safe_malloc(len + 1);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

void	*ft_memset(void *dst, int c, size_t n)
{
    size_t i = 0;
    unsigned char *p = (unsigned char *)dst;
    while (i < n)
        p[i++] = (unsigned char)c;
    return dst;
}

char	*ft_strchr(const char *s, int ch)
{
    unsigned char c = (unsigned char)ch;
    if (!s)
        return NULL;
    while (*s)
    {
        if ((unsigned char)*s == c)
            return (char *)s;
        s++;
    }
    if (c == '\0')
        return (char *)s;
    return NULL;
}
