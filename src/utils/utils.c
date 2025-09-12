#include "minishell.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		write(2, "minishell: malloc failed\n", 25);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

void	ft_putstr_fd(const char *s, int fd)
{
    if (!s)
        return;
    (void)write(fd, s, ft_strlen(s));
}

/* very small formatter: fmt must contain exactly one %s placeholder */
void	ft_dprintf1(int fd, const char *fmt_s, const char *arg)
{
    const char *p;
    if (!fmt_s)
        return;
    p = fmt_s;
    while (*p && *p != '%')
        p++;
    /* write prefix */
    (void)write(fd, fmt_s, (size_t)(p - fmt_s));
    /* skip '%s' if present */
    if (*p == '%' && *(p + 1) == 's')
    {
        ft_putstr_fd(arg ? arg : "(null)", fd);
        p += 2;
    }
    /* write the rest */
    ft_putstr_fd(p, fd);
}

