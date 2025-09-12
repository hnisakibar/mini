#include "minishell.h"

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
	return (read_noninteractive_line());
}
