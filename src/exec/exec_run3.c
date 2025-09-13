#include "minishell.h"
#include "exec_int.h"

int is_meta_only(const char *s)
{
	if (!s || !*s)
		return (0);
	while (*s)
	{
		if (*s != '|' && *s != '<' && *s != '>')
			return (0);
		s++;
	}
	return (1);
}

int has_sentinel(const char *s)
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

int has_dollar(const char *s)
{
	return (s && strchr(s, '$') != NULL);
}

int needs_reparse(char **orig, char **argv)
{
    (void)orig;
    (void)argv;
    return (0);
}

size_t calc_join_len(char **argv)
{
	size_t len = 0;
	int i = 0;
	while (argv[i])
	{
		len += ft_strlen(argv[i]);
		if (argv[i + 1])
			len += 1;
		i++;
	}
	return len;
}
