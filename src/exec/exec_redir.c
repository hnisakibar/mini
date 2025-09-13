#include "minishell.h"

int open_tty_rw(int *rfd, int *wfd)
{
	int rf = open("tty", O_RDONLY);
	int wf = open("tty", O_WRONLY);
	if (rf < 0 || wf < 0)
	{
		if (rf >= 0) close(rf);
		if (wf >= 0) close(wf);
		*rfd = -1;
		*wfd = -1;
		return (-1);
	}
	*rfd = rf;
	*wfd = wf;
	return (0);
}

void hd_prompt_fd(int tty_wfd)
{
	if (tty_wfd >= 0)
		(void)write(tty_wfd, "> ", 2);
}

int dup_to(int fd, int target)
{
	if (dup2(fd, target) < 0)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int redir_in(const char *path, int target_fd)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
        return (perror(path), 1);
    return dup_to(fd, (target_fd >= 0 ? target_fd : STDIN_FILENO));
}

int redir_out_trunc(const char *path, int target_fd)
{
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return (perror(path), 1);
    return dup_to(fd, (target_fd >= 0 ? target_fd : STDOUT_FILENO));
}
