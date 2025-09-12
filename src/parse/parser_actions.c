#include "minishell.h"
#include "parser_int.h"

int ensure_pipe_ok(t_cmd *cur)
{
    /* Allow redirection-only commands (no argv but has redirs) inside pipelines.
       Error only if there is truly no command nor redirection payload. */
    if (!cur || (!cur->argv && !cur->redirs))
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
        return (2);
    }
    return (0);
}

int handle_redir(t_tokarr *ta, size_t *i, t_cmd *cur)
{
    t_toktype   next_t;
    t_redir_type rt;

    if (*i + 1 >= ta->n || ta->v[*i + 1].type != T_WORD)
    {
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
    rt = redir_kind(ta->v[*i].type);
    redir_push(&cur->redirs, rt, ta->v[*i + 1].val);
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
		fprintf(stderr,
			"minishell: syntax error near unexpected token `newline'\n");
		return (2);
	}
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
	{
		argv_push(&cp->cur->argv, ta->v[*i].val);
		(*i)++;
		return (0);
	}
	if (tt == T_LT || tt == T_GT || tt == T_DGT || tt == T_HEREDOC)
		return handle_redir(ta, i, cp->cur);
	(*i)++;
	return (0);
}
