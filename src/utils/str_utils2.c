#include "minishell.h"

char	*ft_strjoin(const char *a, const char *b)
{
	size_t	len_a;
	size_t	len_b;
	char	*res;
	size_t	i;
	size_t	j;

	if (!a && !b)
		return (NULL);
	len_a = ft_strlen(a);
	len_b = ft_strlen(b);
	res = (char *)safe_malloc(len_a + len_b + 1);
	i = 0;
	while (i < len_a)
	{
		res[i] = a[i];
		i++;
	}
	j = 0;
	while (j < len_b)
		res[i++] = b[j++];
	res[i] = '\0';
	return (res);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (!dst && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((const unsigned char *)src)[i];
		i++;
	}
	return (dst);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	slen;
	char	*res;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start >= slen)
		return (ft_strdup(""));
	if (len > slen - start)
		len = slen - start;
	res = (char *)safe_malloc(len + 1);
	i = 0;
	while (i < len)
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
