#include "minishell.h"

static char	*read_one_line_nonint_with_quotes(void);

int	is_interactive(void)
{
	return (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO));
}

static char	*grow_line_buf(char *buf, size_t *cap, size_t len)
{
	char	*nbuf;

	nbuf = (char *)safe_malloc((*cap) * 2);
	ft_memcpy(nbuf, buf, len);
	free(buf);
	*cap *= 2;
	return (nbuf);
}

static ssize_t	read_into_buf(char **pbuf, size_t *pcap, size_t *plen)
{
	char		ch;
	ssize_t	r;

	while ((r = read(STDIN_FILENO, &ch, 1)) == 1)
	{
		if (ch == '\n' || ch == '\r')
			break;
		if (*plen + 1 >= *pcap)
			*pbuf = grow_line_buf(*pbuf, pcap, *plen);
		(*pbuf)[(*plen)++] = ch;
	}
	return (r);
}

static char	*read_noninteractive_line(void)
{
	char		*buf;
	size_t		cap;
	size_t		len;
	ssize_t		r;

	cap = 128;
	len = 0;
	buf = (char *)safe_malloc(cap);
	r = read_into_buf(&buf, &cap, &len);
	if (len == 0 && r != 1)
	{
		free(buf);
		return (NULL);
	}
	buf[len] = '\0';
	return (buf);
}

char	*read_one_line(void)
{
	char	*line;

	if (is_interactive())
	{
		line = readline("minishell$ ");
		if (line && *line)
			add_history(line);
		return (line);
	}
	return (read_one_line_nonint_with_quotes());
}

/* ---- helpers to keep functions short while preserving behavior ---- */

static void	update_quote_state(const char *buf, size_t *i,
                            int *single_q, int *double_q)
{
    while (buf[*i])
    {
        if (!*double_q && buf[*i] == '\'')
            *single_q = !*single_q;
        else if (!*single_q && buf[*i] == '\"')
            *double_q = !*double_q;
        (*i)++;
    }
}

static int	quotes_open(char *buf, size_t *i, int *sq, int *dq)
{
    update_quote_state(buf, i, sq, dq);
    return (*sq || *dq);
}

static ssize_t	append_newline_and_read(char **pbuf, size_t *cap, size_t *len)
{
    if (*len + 1 >= *cap)
        *pbuf = grow_line_buf(*pbuf, cap, *len);
    (*pbuf)[(*len)++] = '\n';
    return read_into_buf(pbuf, cap, len);
}

static char	*read_more_if_quotes_open(char *buf)
{
    size_t  cap;
    size_t  len;
    ssize_t r;
    size_t  i;
    int     sq;
    int     dq;

    cap = ft_strlen(buf) + 1;
    if (cap < 128)
        cap = 128;
    len = ft_strlen(buf);
    sq = 0;
    dq = 0;
    i = 0;
    while (quotes_open(buf, &i, &sq, &dq))
    {
        r = append_newline_and_read(&buf, &cap, &len);
        buf[len] = '\0';
        if (r != 1)
            break ;
    }
    return (buf);
}

static char	*read_one_line_nonint_with_quotes(void)
{
    char *line;
	
	line = read_noninteractive_line();
    if (!line)
        return NULL;
    return read_more_if_quotes_open(line);
}
