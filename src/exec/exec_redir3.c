/* heredoc helpers to keep functions within Norm limits */
#include "minishell.h"

static void	setup_heredoc(struct sigaction *oldint, int *tty_r, int *tty_w)
{
    struct sigaction act;

    ft_memset(&act, 0, sizeof(act));
    act.sa_handler = hd_sigint_handler;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, oldint);
    *tty_r = -1;
    *tty_w = -1;
    if (is_interactive())
        open_tty_rw(tty_r, tty_w);
    if (*tty_r < 0)
        *tty_w = -1;
    g_signal = 0;
}

static void	cleanup_heredoc(struct sigaction *oldint, int tty_r, int tty_w)
{
    if (tty_w >= 0)
        close(tty_w);
    if (tty_r >= 0)
        close(tty_r);
    sigaction(SIGINT, oldint, NULL);
}

static void	write_heredoc_line(int wfd, char *line, int quoted,
                            t_env *env, int last_status)
{
    if (!quoted)
    {
        char *expanded = expand_word(line, env, last_status);
        write_line(wfd, expanded);
        free(expanded);
    }
    else
        write_line(wfd, line);
}

int heredoc_fill(int wfd, const char *delim, t_env *env,
                 int last_status, int quoted)
{
    int                 tty_r;
    int                 tty_w;
    char               *line;
    struct sigaction    oldint;

    setup_heredoc(&oldint, &tty_r, &tty_w);
    line = NULL;
    while (!g_signal)
    {
        if (tty_w >= 0)
            hd_prompt_fd(tty_w);
        line = read_line_dynamic_fd(tty_r >= 0 ? tty_r : STDIN_FILENO);
        if (!line)
            break ;
        if (ft_strcmp(line, delim) == 0)
        {
            free(line);
            break ;
        }
        write_heredoc_line(wfd, line, quoted, env, last_status);
        free(line);
    }
    cleanup_heredoc(&oldint, tty_r, tty_w);
    return (g_signal ? -1 : 0);
}


int do_heredoc(const char *delim, int quoted, t_env *env, int last_status)
{
	int pfd[2];
	if (heredoc_pipe(pfd) < 0)
		return (-1);
    if (heredoc_fill(pfd[1], delim, env, last_status, quoted) != 0)
	{
		close(pfd[1]);
		close(pfd[0]);
		return (-1);
	}
	close(pfd[1]);
	return (pfd[0]);
}

char *strip_sentinels(const char *s)
{
    size_t  i;
    size_t  j;
    size_t  len;
    char   *out;

    if (!s)
        return (NULL);
    i = 0;
    j = 0;
    len = ft_strlen(s);
    out = (char *)malloc(len + 1);
    if (!out)
        return (NULL);
    while (s[i])
    {
        if ((unsigned char)s[i] == 0x01 || (unsigned char)s[i] == 0x02)
        { i++; continue; }
        out[j++] = s[i++];
    }
    out[j] = '\0';
    return (out);
}

static int	apply_file_redir(const t_redir *r, t_env *env,
                        int last_status, char **pclean)
{
    char       *expanded;
    const char *used;
    int         err;
    int         fd_from;

    expanded = expand_word(r->arg, env, last_status);
    *pclean = strip_sentinels(expanded);
    free(expanded);
    used = (*pclean ? *pclean : r->arg);
    fd_from = (r->fd_from >= 0 ? r->fd_from : -1);
    err = 0;
    if (r->type == R_IN)
        err = redir_in(used, fd_from);
    else if (r->type == R_OUT_TRUNC)
        err = redir_out_trunc(used, fd_from);
    else
        err = redir_out_append(used, fd_from);
    return (err);
}

static int	apply_heredoc_redir(const t_redir *r, t_env *env,
                           int last_status, char **pclean)
{
    const char *used;
    int         rfd;
    int         err;
    int         was_quoted;
    int         fd_from;

    was_quoted = r->quoted;
    *pclean = strip_sentinels(r->arg);
    used = (*pclean ? *pclean : r->arg);
    rfd = do_heredoc(used, was_quoted, env, last_status);
    fd_from = (r->fd_from >= 0 ? r->fd_from : STDIN_FILENO);
    if (rfd < 0 || dup_to(rfd, fd_from))
        err = (rfd < 0) ? 130 : 1;
    else
        err = 0;
    return (err);
}

int apply_redirs(const t_cmd *c, t_env *env, int last_status)
{
    const t_redir *r;

    r = c->redirs;
    while (r)
    {
        char *clean;
        int   err;

        clean = NULL;
        if (r->type == R_IN || r->type == R_OUT_TRUNC || r->type == R_OUT_APPEND)
            err = apply_file_redir(r, env, last_status, &clean);
        else
            err = apply_heredoc_redir(r, env, last_status, &clean);
        free(clean);
        if (err)
            return (err);
        r = r->next;
    }
    return (0);
}
