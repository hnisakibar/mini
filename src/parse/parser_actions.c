#include "minishell.h"
#include "parser_int.h"

static int is_all_digits(const char *s)
{
    if (!s || !*s) return 0;
    for (const unsigned char *p = (const unsigned char *)s; *p; ++p)
        if (!ft_isdigit(*p)) return 0;
    return 1;
}

int ensure_pipe_ok(t_cmd *cur)
{
    if (!cur || (!cur->argv && !cur->redirs))
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
        return (2);
    }
    return (0);
}

static int	report_next_syntax_err(t_tokarr *ta, size_t *i)
{
    t_toktype next_t;

    if (*i + 1 >= ta->n)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
        return (2);
    }
    next_t = ta->v[*i + 1].type;
    ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
    ft_putstr_fd(tok_repr(next_t), STDERR_FILENO);
    ft_putstr_fd("'\n", STDERR_FILENO);
    return (2);
}

static void	push_redir_node(t_cmd *cur, t_tokarr *ta, size_t i,
                            t_redir_type rt, int fd_from)
{
    if (rt == R_HEREDOC)
        redir_push(&cur->redirs, rt, fd_from, ta->v[i + 1].val, ta->v[i + 1].had_quote);
    else
        redir_push(&cur->redirs, rt, fd_from, ta->v[i + 1].val, 0);
}

int handle_redir(t_tokarr *ta, size_t *i, t_cmd *cur, int *pending_fd)
{
    t_redir_type rt;
    int          fd_from;

    if (*i + 1 >= ta->n || ta->v[*i + 1].type != T_WORD)
        return (report_next_syntax_err(ta, i));
    rt = redir_kind(ta->v[*i].type);
    fd_from = (*pending_fd >= 0 ? *pending_fd : -1);
    *pending_fd = -1;
    push_redir_node(cur, ta, *i, rt, fd_from);
    *i += 2;
    return (0);
}

static int handle_pipe_token(t_tokarr *ta, size_t *i, t_cmdpair *cp)
{
	if (ensure_pipe_ok(cp->cur))
		return (2);
	cp->last = cp->cur;
	cp->cur = NULL;
	(*i)++;
	if (*i >= ta->n)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (2);
	}
	return (0);
}

static int	is_redir_tok(t_toktype tt)
{
    return (tt == T_LT || tt == T_GT || tt == T_DGT || tt == T_HEREDOC);
}

static int	handle_word_token(t_tokarr *ta, size_t *i, t_cmdpair *cp)
{
    if ((*i + 1) < ta->n && is_redir_tok(ta->v[*i + 1].type)
        && is_all_digits(ta->v[*i].val) && ta->v[*i].glued_right)
    {
        cp->pending_fd = ft_atoi(ta->v[*i].val);
        (*i)++;
        return (0);
    }
    argv_push(&cp->cur->argv, ta->v[*i].val);
    (*i)++;
    return (0);
}

int handle_token(t_tokarr *ta, size_t *i,
	t_pipeline *pl, t_cmdpair *cp)
{
	t_toktype tt = ta->v[*i].type;

	if (tt == T_PIPE)
		return handle_pipe_token(ta, i, cp);
	attach_cmd(pl, cp);
    if (tt == T_WORD)
        return (handle_word_token(ta, i, cp));
    if (is_redir_tok(tt))
        return (handle_redir(ta, i, cp->cur, &cp->pending_fd));
    (*i)++;
    return (0);
}
