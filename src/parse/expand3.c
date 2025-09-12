#include "minishell.h"


int has_ifs_space(const char *s)
{
	while (s && *s)
	{
		if (*s == ' ' || *s == '\t')
			return 1;
		s++;
	}
	return 0;
}
void ensure_out_cap(char ***pargv, int *pcap, int need)
{
	if (need <= *pcap) return;
	int newcap = (*pcap > 0 ? *pcap * 2 : need + 8);
	if (newcap < need) newcap = need;

	char **newv = safe_malloc((size_t)newcap * sizeof(char *));
	if (*pargv && *pcap > 0)
		ft_memcpy(newv, *pargv, (size_t)(*pcap) * sizeof(char *));
	free(*pargv);
	*pargv = newv;
	*pcap = newcap;
}

void process_field_split(const char *expanded, char ***out, int *m, int *cap)
{
	const char *p = expanded;
	while (*p)
	{
		while (*p == ' ' || *p == '\t') p++;
		if (!*p) break;

		const char *q = p;
		while (*q && *q != ' ' && *q != '\t') q++;

		size_t len = (size_t)(q - p);
		char *field = (char *)safe_malloc(len + 1);
		ft_memcpy(field, p, len);
		field[len] = '\0';

		ensure_out_cap(out, cap, *m + 2);
		(*out)[(*m)++] = field;
		p = q;
	}
}
void process_expanded_arg(char ***out, int *m, int *cap,
	const char *original_arg, char *expanded)
{
	if (is_pure_var_expr(original_arg) && has_ifs_space(expanded))
	{
		process_field_split(expanded, out, m, cap);
		free(expanded);
	}
	else if (expanded[0] == '\0' && is_pure_var_expr(original_arg))
	{
		free(expanded);
	}
	else
	{
		ensure_out_cap(out, cap, *m + 2);
		(*out)[(*m)++] = expanded;
	}
}
char **expand_argv(char **argv, t_env *env, int last_status)
{
	int i = 0, m = 0, cap = 8;
	char **out = safe_malloc(sizeof(char *) * cap);

	while (argv && argv[i])
	{
		char *expanded = expand_word(argv[i], env, last_status);
		process_expanded_arg(&out, &m, &cap, argv[i], expanded);
		i++;
	}
	ensure_out_cap(&out, &cap, m + 1);
	out[m] = NULL;
	return out;
}
