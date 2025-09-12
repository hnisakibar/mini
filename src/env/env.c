#include "minishell.h"

int		env_set(t_env **env, const char *key, const char *val, int ow)
{
	t_env	*e;

	e = *env;
	while (e)
	{
		if (ft_strcmp(e->key, key) == 0)
		{
			if (ow)
			{
				free(e->val);
				e->val = (val ? ft_strdup(val) : NULL);
			}
			return (0);
		}
		e = e->next;
	}
	e = new_var(key, val);
	e->next = *env;
	*env = e;
	return (0);
}

int		env_unset(t_env **env, const char *key)
{
	t_env	*p;
	t_env	*c;

	p = NULL;
	c = *env;
	while (c)
	{
		if (ft_strcmp(c->key, key) == 0)
		{
			if (p)
				p->next = c->next;
			else
				*env = c->next;
			free(c->key);
			free(c->val);
			free(c);
			return (0);
		}
		p = c;
		c = c->next;
	}
	return (0);
}

char	**env_to_environ(t_env *env)
{
	int		n;
	int		i;
	t_env	*it;
	char	**arr;

	n = 0;
	it = env;
	while (it)
	{
		n++;
		it = it->next;
	}
	arr = (char **)safe_malloc(sizeof(char *) * (n + 1));
	i = 0;
	it = env;
	while (it)
	{
		arr[i++] = make_pair(it->key, it->val ? it->val : "");
		it = it->next;
	}
	arr[i] = NULL;
	return (arr);
}

static void set_numeric_var(t_env **env, const char *key, long val)
{
    char buf[32];
    int n = ft_ltoa_buf(val, buf, sizeof(buf));
    if (n > 0)
        env_set(env, key, buf, 1);
}

void    env_normalize_startup(t_env **env)
{
    const char *sh;
    char       *endp;
    long        v;

    
    errno = 0;
    endp = NULL;
    sh = env_get(*env, "SHLVL");
    if (!sh)
        set_numeric_var(env, "SHLVL", 1);
    else
    {
        v = strtol(sh, &endp, 10);
        if (errno == ERANGE || endp == sh)
            set_numeric_var(env, "SHLVL", 1);
        else
            set_numeric_var(env, "SHLVL", v + 1);
    }
    
    env_unset(env, "_");
}

