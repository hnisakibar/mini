#include "minishell.h"

size_t	env_len(const t_env *e)
{
	size_t	n;

	n = 0;
	while (e)
	{
		n++;
		e = e->next;
	}
	return (n);
}

void	env_collect(const t_env *e, const t_env **arr)
{
	size_t	i;

	i = 0;
	while (e)
	{
		arr[i++] = e;
		e = e->next;
	}
}

void	env_sort(const t_env **a, size_t n)
{
	size_t	i;
	size_t	j;
	const t_env	*t;

	i = 0;
	while (i + 1 < n)
	{
		j = 0;
		while (j + 1 < n)
		{
			if (strcmp(a[j]->key, a[j + 1]->key) > 0)
			{ t = a[j]; a[j] = a[j + 1]; a[j + 1] = t; }
			j++;
		}
		i++;
	}
}

void	env_print_decl(const t_env **a, size_t n)
{
    size_t	i;

    i = 0;
    while (i < n)
    {
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putstr_fd(a[i]->key, STDOUT_FILENO);
        if (a[i]->val)
        {
            ft_putstr_fd("=\"", STDOUT_FILENO);
            ft_putstr_fd(a[i]->val, STDOUT_FILENO);
            ft_putstr_fd("\"\n", STDOUT_FILENO);
        }
        else
            ft_putstr_fd("\n", STDOUT_FILENO);
        i++;
    }
}

int	valid_key(const char *s, size_t len)
{
	size_t	i;

    if (len == 0 || !(s[0] == '_' || ft_isalpha((unsigned char)s[0])))
		return (0);
	i = 1;
	while (i < len)
	{
        if (!(s[i] == '_' || ft_isalnum((unsigned char)s[i])))
			return (0);
		i++;
	}
	return (1);
}
