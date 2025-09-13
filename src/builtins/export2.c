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
			if (ft_strcmp(a[j]->key, a[j + 1]->key) > 0)
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

void	export_set_pair(t_env **env, const char *k, size_t klen, const char *v)
{
	char	*key;
	key = (char *)malloc(klen + 1);
	if (!key)
		return ;
	ft_memcpy(key, k, klen);
	key[klen] = '\0';
	env_set(env, key, v, 1);
	free(key);
}

static int	export_append_value(t_env **env, const char *arg,
								 size_t klen, const char *rhs)
{
	char		*key;
	const char	*oldv;
	size_t		lo;
	size_t		lr;
	char		*nv;

	key = (char *)malloc(klen + 1);
	if (!key)
		return (1);
	ft_memcpy(key, arg, klen);
	key[klen] = '\0';
	oldv = env_get(*env, key);
	lr = ft_strlen(rhs);
	lo = (oldv ? ft_strlen(oldv) : 0);
	nv = (char *)safe_malloc(lo + lr + 1);
	if (oldv)
		ft_memcpy(nv, oldv, lo);
	ft_memcpy(nv + lo, rhs, lr);
	nv[lo + lr] = '\0';
	env_set(env, key, nv, 1);
	free(nv);
	free(key);
	return (0);
}

int	export_assign(t_env **env, const char *arg, size_t klen, int append,
				   const char *rhs)
{
	if (append)
		return export_append_value(env, arg, klen, rhs);
	export_set_pair(env, arg, klen, rhs);
	return (0);
}

int	export_handle_option(const char *opt)
{
	if (opt[1] == '\0')
	{
		ft_putstr_fd("minishell: export: -: invalid option\n", STDERR_FILENO);
		ft_putstr_fd(
			"export: usage: export [-fn] [name[=value] ...] or export -p\n",
			STDERR_FILENO);
		return (2);
	}
	if (opt[1] == '-' && opt[2] == '\0')
		return (1);
	if (opt[1] == '-')
		ft_putstr_fd("minishell: export: --: invalid option\n", STDERR_FILENO);
	else
	{
		ft_putstr_fd("minishell: export: -", STDERR_FILENO);
		write(STDERR_FILENO, &opt[1], 1);
		ft_putstr_fd(": invalid option\n", STDERR_FILENO);
	}
	ft_putstr_fd(
		"export: usage: export [-fn] [name[=value] ...] or export -p\n",
		STDERR_FILENO);
	return (2);
}
