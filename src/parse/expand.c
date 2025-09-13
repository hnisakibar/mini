#include "minishell.h"

char *grow_buf(char *ptr, size_t old_len, size_t new_cap)
{
	char *new_buf = (char *)safe_malloc(new_cap);
	if (ptr && old_len)
		ft_memcpy(new_buf, ptr, old_len);
	free(ptr);
	return new_buf;
}

size_t safe_cap(size_t used, size_t add)
{
	if (used > (size_t)-1 - (add + 1))
	{
		write(2, "minishell: expand overflow\n", 27);
		exit(EXIT_FAILURE);
	}
	return used + add + 1;
}

void app_str(char **dst, size_t *len, const char *src, size_t n)
{
	size_t cap = safe_cap(*len, n);
	*dst = grow_buf(*dst, *len, cap);
	ft_memcpy(*dst + *len, src, n);
	*len += n;
	(*dst)[*len] = '\0';
}

void app_c(char **dst, size_t *len, char c)
{
	size_t cap = safe_cap(*len, 1);
	*dst = grow_buf(*dst, *len, cap);
	(*dst)[(*len)++] = c;
	(*dst)[*len] = '\0';
}

void push_status(char **out, size_t *len, int last_status)
{
	char buf[32];
	int n = ft_itoa_buf(last_status, buf, sizeof(buf));
	if (n > 0)
		app_str(out, len, buf, (size_t)n);
}
