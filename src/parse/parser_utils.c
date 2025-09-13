#include "minishell.h"

void	*xcalloc(size_t n, size_t sz)
{
	void	*p;

    p = safe_malloc(n * sz);
    ft_memset(p, 0, n * sz);
	return (p);
}

char	*strndup2(const char *s, size_t n)
{
	char	*p;

	p = safe_malloc(n + 1);
    ft_memcpy(p, s, n);
	p[n] = '\0';
	return (p);
}

char	*strdup2(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (strndup2(s, len));
}

void	argv_push(char ***pargv, const char *w)
{
	size_t	n;
	size_t	i;
	char	**nv;

	n = 0;
	if (*pargv)
		while ((*pargv)[n])
			n++;
	nv = xcalloc(n + 2, sizeof(char *));
	i = 0;
	while (i < n)
	{
		nv[i] = (*pargv)[i];
		i++;
	}
	nv[n] = strdup2(w);
	nv[n + 1] = NULL;
	free(*pargv);
	*pargv = nv;
}

void	redir_push(t_redir **lst, t_redir_type type, int fd_from, const char *arg, int quoted)
{
	t_redir	*r;
	t_redir	*p;

	r = xcalloc(1, sizeof(*r));
	r->type = type;
	r->arg = strdup2(arg);
    r->fd_from = fd_from;
    r->quoted = quoted;
	if (!*lst)
	{
		*lst = r;
		return ;
	}
	p = *lst;
	while (p->next)
		p = p->next;
	p->next = r;
}
