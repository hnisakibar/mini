#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include "ast.h"

void    *safe_malloc(size_t size);
size_t  ft_strlen(const char *s);
char    *ft_strdup(const char *s);
char    *ft_strjoin(const char *a, const char *b);
void    *ft_memcpy(void *dst, const void *src, size_t n);
void    *ft_memset(void *dst, int c, size_t n);
char    *ft_substr(const char *s, unsigned int start, size_t len);
int     ft_itoa_buf(int n, char *buf, size_t size);
int     ft_ltoa_buf(long n, char *buf, size_t size);
int     argv_len(char **argv);
int     ft_isalpha(int c);
int     ft_isalnum(int c);
int     ft_isdigit(int c);
char    *ft_strchr(const char *s, int c);
int     ft_strcmp(const char *a, const char *b);
void    ft_putstr_fd(const char *s, int fd);
void    ft_dprintf1(int fd, const char *fmt_s, const char *arg);
void    *xcalloc(size_t n, size_t sz);
void    argv_push(char ***pargv, const char *w);
void    redir_push(t_redir **lst, t_redir_type type, const char *arg);

#endif


