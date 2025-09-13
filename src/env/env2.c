#include "minishell.h"

t_env	*new_var(const char *k, const char *v)
{
	t_env	*e;

	e = (t_env *)safe_malloc(sizeof(*e));
	e->key = ft_strdup(k);
	e->val = (v ? ft_strdup(v) : NULL);
	e->next = NULL;
	return (e);
}

char		*make_pair(const char *k, const char *v)
{
	size_t	lk;
	size_t	lv;
	char	*s;

    lk = ft_strlen(k);
    lv = (v ? ft_strlen(v) : 0);
	s = (char *)safe_malloc(lk + 1 + lv + 1);
	ft_memcpy(s, k, lk);
	s[lk] = '=';
	if (lv)
		ft_memcpy(s + lk + 1, v, lv);
	s[lk + 1 + lv] = '\0';
	return (s);
}

t_env	*create_env_node(char *entry)
{
	char	*dup;
	char	*eq;
	t_env	*node;

	dup = ft_strdup(entry);
	if (!dup)
		return (NULL);
	eq = ft_strchr(dup, '=');
	if (!eq)
	{
		free(dup);
		return (NULL);
	}
	*eq = '\0';
	node = new_var(dup, eq + 1);
	free(dup);
	return (node);
}

t_env	*env_init(char **environ)
{
	t_env	*head = NULL;
	t_env	*tail = NULL;
	t_env	*new;
	int		i = 0;

	while (environ && environ[i])
	{
		new = create_env_node(environ[i]);
		if (new)
		{
			if (!head)
				head = tail = new;
			else
			{
				tail->next = new;
				tail = new;
			}
		}
		i++;
	}
	return (head);
}

char	*env_get(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->val);
		env = env->next;
	}
	return (NULL);
}
