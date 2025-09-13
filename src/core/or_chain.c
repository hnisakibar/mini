#include "minishell.h"

int	find_unquoted_or(const char *s)
{
	int	single_quote;
	int	double_quote;
	int	i;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	while (s[i])
	{
		if (!double_quote && s[i] == '\'')
			single_quote = !single_quote;
		else if (!single_quote && s[i] == '\"')
			double_quote = !double_quote;
		else if (!single_quote && !double_quote && s[i] == '|' && s[i + 1] == '|')
			return (i);
		i++;
	}
	return (-1);
}

int	find_unquoted_semi(const char *s)
{
    int single_quote = 0, double_quote = 0;
    int i = 0;
    while (s[i])
    {
        if (!double_quote && s[i] == '\'') single_quote = !single_quote;
        else if (!single_quote && s[i] == '\"') double_quote = !double_quote;
        else if (!single_quote && !double_quote && s[i] == ';')
            return i;
        i++;
    }
    return -1;
}

static char *strtrim2(const char *s, size_t n)
{
    /* trim leading/trailing spaces/tabs */
    size_t b = 0, e = n;
    while (b < n && (s[b] == ' ' || s[b] == '\t')) b++;
    while (e > b && (s[e - 1] == ' ' || s[e - 1] == '\t')) e--;
    return strndup2(s + b, e - b);
}

static void run_segment(const char *seg, t_ctx *ctx, char **envp)
{
    if (!seg || !*seg)
        return;
    if (find_unquoted_or(seg) >= 0)
        run_or_chain((char *)seg, ctx, envp);
    else
        run_line(seg, ctx, envp);
}

int run_seq_chain(char *line, t_ctx *ctx, char **envp)
{
    int idx;
    char *rest;
    char *seg;

    rest = line;
    while ((idx = find_unquoted_semi(rest)) >= 0)
    {
        seg = strtrim2(rest, (size_t)idx);
        run_segment(seg, ctx, envp);
        free(seg);
        rest = rest + idx + 1;
    }
    if (rest && *rest)
    {
        seg = strtrim2(rest, ft_strlen(rest));
        run_segment(seg, ctx, envp);
        free(seg);
    }
    return 0;
}

static int	split_or_parts(const char *line, char **left, char **right)
{
    int idx;

    *left = NULL;
    *right = NULL;
    idx = find_unquoted_or(line);
    if (idx < 0)
        return (-1);
    *left = strndup2(line, (size_t)idx);
    *right = ft_strdup(line + idx + 2);
    if (!*left || !*right)
        return (free(*left), free(*right), *left = NULL, *right = NULL, 0);
    return (0);
}

static int	exec_left_status(const char *left, t_ctx *ctx, char **envp)
{
    t_pipeline *pl;
    int         perr;

    pl = NULL;
    perr = parse_line(left, &pl);
    if (perr == 0 && pl && pl->head)
        perr = execute_pipeline(pl, ctx, envp);
    free_pipeline(pl);
    return (perr);
}

int	run_or_chain(char *line, t_ctx *ctx, char **envp)
{
    char    *left;
    char    *right;
    int     st;

    if (split_or_parts(line, &left, &right) < 0)
        return (-1);
    st = exec_left_status(left, ctx, envp);
    if (st != 0)
        run_line(right, ctx, envp);
    else
        ctx->last_status = 0;
    free(left);
    free(right);
    return (0);
}
