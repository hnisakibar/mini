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
