#include "minishell.h"

int redir_out_append(const char *path, int target_fd)
{
	int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(path), 1);
	return dup_to(fd, (target_fd >= 0 ? target_fd : STDOUT_FILENO));
}

int heredoc_pipe(int pfd[2])
{
	if (pipe(pfd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

void write_line(int wfd, const char *s)
{
	size_t len = ft_strlen(s);
	if (len)
		(void)write(wfd, s, len);
	(void)write(wfd, "\n", 1);
}

char *read_line_dynamic_fd(int fd)
{
	size_t cap = 1024, len = 0;
	char *buf = malloc(cap), ch;
	ssize_t r;

	if (!buf) return NULL;
	while ((r = read(fd, &ch, 1)) == 1)
	{
		if (len + 1 >= cap)
		{
			size_t ncap = cap * 2;
			char *tmp = malloc(ncap);
			if (!tmp) return free(buf), NULL;
			ft_memcpy(tmp, buf, cap);
			free(buf);
			buf = tmp; cap = ncap;
		}
		if (ch == '\n') break;
		buf[len++] = ch;
	}
	buf[len] = '\0';
	if (len == 0 && r != 1) return free(buf), NULL;
	return buf;
}

void hd_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
}
