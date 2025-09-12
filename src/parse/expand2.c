#include "minishell.h"

size_t varname_len(const char *s)
{
    if ((unsigned char)s[0] == 0x01 || (unsigned char)s[0] == 0x02)
        return 0;
    if (!(s[0] == '_' || isalpha((unsigned char)s[0])))
        return 0;
    size_t i = 1;
    while ((unsigned char)s[i] != 0x01 && (unsigned char)s[i] != 0x02 &&
           (s[i] == '_' || isalnum((unsigned char)s[i])))
        i++;
    return i;
}
int expand_dollar(const char *word, size_t *i, char **out, size_t *len, t_env *env, int last_status)
{
	if (word[*i + 1] == '\0') return 0;
	if (word[*i + 1] == '?')
		return (push_status(out, len, last_status), *i += 2, 1);

	size_t klen = varname_len(&word[*i + 1]);
	if (klen == 0) return 0;

	char *key = safe_malloc(klen + 1);
	ft_memcpy(key, &word[*i + 1], klen);
	key[klen] = '\0';

	const char *val = env_get(env, key);
	if (val)
		app_str(out, len, val, ft_strlen(val));
	free(key);
	*i += 1 + klen;
	return 1;
}

void app_home(char **out, size_t *len, t_env *env)
{
	const char *home = env_get(env, "HOME");
	if (home && *home)
		app_str(out, len, home, ft_strlen(home));
	else
		app_c(out, len, '~');
}
char *expand_word(const char *w, t_env *env, int last_status)
{
    char *out = NULL;
    size_t len = 0, i = 0;

    while (w[i])
    {
        if ((unsigned char)w[i] == 0x01)
        {
            i++;
            if (w[i])
                app_c(&out, &len, w[i++]);
            continue;
        }
        /* 0x02 is a boundary marker (beginning of a double-quoted chunk).
           It should not force literal copying; just skip it so that the
           following characters are handled normally (allowing $ inside
           double quotes to expand). */
        if ((unsigned char)w[i] == 0x02)
        {
            i++;
            continue;
        }
        if (i == 0 && w[i] == '~' && (w[i + 1] == '\0' || w[i + 1] == '/'))
        {
            app_home(&out, &len, env);
            i++;
            continue;
        }
        if (w[i] == '$' && expand_dollar(w, &i, &out, &len, env, last_status))
            continue;
        app_c(&out, &len, w[i++]);
    }
    return out ? out : ft_strdup("");
}
int is_pure_var_expr(const char *w)
{
	size_t i = 0;
	int had = 0;
	while (w[i])
	{
		if (w[i] != '$') return 0;
		if (w[i + 1] == '?') { i += 2; had = 1; continue; }

		size_t k = varname_len(&w[i + 1]);
		if (k == 0) return 0;
		i += 1 + k;
		had = 1;
	}
	return had;
}
