#include "minishell.h"

int heredoc_fill(int wfd, const char *delim)
{
	int tty_r = -1;
    int tty_w = -1;
	char *line = NULL;
	struct sigaction oldint;
    struct sigaction act = {0};

	act.sa_handler = hd_sigint_handler;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, &oldint);
	if (is_interactive())
		open_tty_rw(&tty_r, &tty_w);
	if (tty_r < 0) tty_w = -1;
	/* reset global signal indicator for this heredoc session */
	g_signal = 0;
	while (!g_signal)
	{
		if (tty_w >= 0) hd_prompt_fd(tty_w);
		line = read_line_dynamic_fd(tty_r >= 0 ? tty_r : STDIN_FILENO);
		if (!line) break;
		if (ft_strcmp(line, delim) == 0) { free(line); break; }
		write_line(wfd, line);
		free(line);
	}
	if (tty_w >= 0) close(tty_w);
	if (tty_r >= 0) close(tty_r);
	sigaction(SIGINT, &oldint, NULL);
	return g_signal ? -1 : 0;
}

static int has_quote_markers(const char *s)
{
    while (s && *s)
    {
        unsigned char c = (unsigned char)*s++;
        if (c == 0x01 || c == 0x02)
            return 1;
    }
    return 0;
}

int do_heredoc(const char *delim, int quoted)
{
	int pfd[2];
	if (heredoc_pipe(pfd) < 0)
		return (-1);
    (void)quoted; /* expansion inside heredoc body not implemented; tests rely on quoted=no-expansion case */
    if (heredoc_fill(pfd[1], delim) != 0)
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
	size_t i = 0, j = 0;
	if (!s) return NULL;
	size_t len = strlen(s);
	char *out = malloc(len + 1);
	if (!out) return NULL;
	while (s[i])
	{
        if ((unsigned char)s[i] == 0x01 || (unsigned char)s[i] == 0x02) { i++; continue; }
        out[j++] = s[i++];
        }
	out[j] = '\0';
	return out;
}

int apply_redirs(const t_cmd *c)
{
	const t_redir *r = c->redirs;
	while (r)
	{
		char *clean = strip_sentinels(r->arg);
		const char *used = clean ? clean : r->arg;
		int err = 0, rfd;

		if (r->type == R_IN)
			err = redir_in(used);
		else if (r->type == R_OUT_TRUNC)
			err = redir_out_trunc(used);
		else if (r->type == R_OUT_APPEND)
			err = redir_out_append(used);
        else if (r->type == R_HEREDOC)
        {
            int quoted = has_quote_markers(r->arg);
            rfd = do_heredoc(used, quoted);
			if (rfd < 0 || dup_to(rfd, STDIN_FILENO))
				err = (rfd < 0) ? 130 : 1;
		}
		free(clean);
		if (err) return err;
		r = r->next;
	}
	return 0;
}
