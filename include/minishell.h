/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekibar <ekibar@student.42istanbul.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 08:00:00 by ekibar            #+#    #+#             */
/*   Updated: 2025/09/11 00:00:00 by ekibar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <errno.h>
# include <limits.h>
# include <ctype.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "ast.h"
# include "env.h"

/* ---- globals ---- */
extern volatile sig_atomic_t g_signal;

/* runtime context (replace most globals) */
typedef struct s_ctx {
    t_env *env;
    int    last_status;
    int    in_parent_exit;
    pid_t *pids;
    int    pids_n;
    t_pipeline *pipeline;
    int    saved_stdin;
    int    saved_stdout;
    int    saved_stderr;
} t_ctx;

/* ---- child helpers ---- */
void  child_exec(const t_cmd *c, t_ctx *ctx, char **envp, int prev_r, int fd[2]);
void  close_pair(int fd0, int fd1);
void  close_if(int fd);

/* ---- parser ---- */
int   parse_line(const char *line, t_pipeline **out);
void  free_pipeline(t_pipeline *p);
int   parse_tokens(t_tokarr *ta, t_pipeline **out);

/* ---- exec ---- */
int   execute_pipeline(const t_pipeline *p, t_ctx *ctx, char **envp);
int   count_cmds(const t_pipeline *p);
int   is_single_builtin(const t_pipeline *p);
int   run_single_builtin(const t_cmd *cmd, t_ctx *ctx, char **envp);
int   open_next_pipe(int need, int fd[2]);
int   after_fork_parent(int i, int n, int prev_r, int fd1, int fd0);
int   wait_all(pid_t *pids, int n, pid_t last_pid);
/* redirections API */
int   apply_redirs(const t_cmd *c, t_env *env, int last_status);
int   open_tty_rw(int *rfd, int *wfd);
void  hd_prompt_fd(int tty_wfd);
int   dup_to(int fd, int target);
int   redir_in(const char *path, int target_fd);
int   redir_out_trunc(const char *path, int target_fd);
int   redir_out_append(const char *path, int target_fd);
int   heredoc_pipe(int pfd[2]);
void  write_line(int wfd, const char *s);
char *read_line_dynamic_fd(int fd);
void  hd_sigint_handler(int sig);
int   heredoc_fill(int wfd, const char *delim, t_env *env, int last_status, int quoted);
int   do_heredoc(const char *delim, int quoted, t_env *env, int last_status);
char *strip_sentinels(const char *s);
int   exec_inplace(char **orig_argv, char **argv, t_ctx *ctx, char **envp);

/* ---- builtins ---- */
int   is_builtin(const char *cmd);
int   run_builtin(char **argv, t_ctx *ctx);
int   msh_builtin_echo(char **argv);
int   msh_builtin_pwd(void);
int   msh_builtin_env(t_env *env);
int   msh_builtin_cd(char **argv, t_env **env);
int   msh_builtin_export(char **argv, t_env **env);
int   msh_builtin_unset(char **argv, t_env **env);
int   msh_builtin_exit(char **argv, t_ctx *ctx);
/* exit helpers (used across exit.c/exit2.c) */
void          print_exit_if_interactive(void);
void          free_argv_inline(char **argv);
void          exit_now(int status, char **argv, t_ctx *ctx);
int           is_numeric_arg(const char *s);
unsigned char parse_code_or_die(const char *s, char **argv, t_ctx *ctx);
/* cd helpers (used across cd.c/cd2.c) */
int           arg_count(char **av);
const char   *target_path(char **av, t_env *env);
void          update_pwd_oldpwd(t_env **env, const char *oldpwd);
int           too_many_args(char **argv);
/* export helpers (used across export.c/export2.c) */
size_t        env_len(const t_env *e);
void          env_collect(const t_env *e, const t_env **arr);
void          env_sort(const t_env **a, size_t n);
void          env_print_decl(const t_env **a, size_t n);
int           valid_key(const char *s, size_t len);
void          export_set_pair(t_env **env, const char *k, size_t klen, const char *v);
int           export_assign(t_env **env, const char *arg, size_t klen, int append,
                 const char *rhs);
int           export_handle_option(const char *opt);

/* ---- core ---- */
void  prompt_signals(void);
char *read_one_line(void);
int   is_interactive(void);
int   handle_parent_exit(t_pipeline *pl, t_ctx *ctx, char **envp);
int   run_or_chain(char *line, t_ctx *ctx, char **envp);
int   find_unquoted_or(const char *s);
int   find_unquoted_semi(const char *s);
int   run_seq_chain(char *line, t_ctx *ctx, char **envp);
int   run_line(const char *line, t_ctx *ctx, char **envp);
void  free_all(t_ctx *ctx);
void  free_env(t_env *env);
void  env_normalize_startup(t_env **env);

/* ---- expansion ---- */
/* expand.c */
char   *grow_buf(char *ptr, size_t old_len, size_t new_cap);
size_t  safe_cap(size_t used, size_t add);
void    app_str(char **dst, size_t *len, const char *src, size_t n);
void    app_c(char **dst, size_t *len, char c);
void    push_status(char **out, size_t *len, int last_status);

/* expand2.c */
size_t  varname_len(const char *s);
int     expand_dollar(const char *word, size_t *i, char **out, size_t *len, t_env *env, int last_status);
void    app_home(char **out, size_t *len, t_env *env);
char   *expand_word(const char *w, t_env *env, int last_status);
int     is_pure_var_expr(const char *w);

/* expand3.c */
int     has_ifs_space(const char *s);
void    ensure_out_cap(char ***pargv, int *pcap, int need);
void    process_field_split(const char *expanded, char ***out, int *m, int *cap);
void    process_expanded_arg(char ***out, int *m, int *cap,
            const char *original_arg, char *expanded);
char  **expand_argv(char **argv, t_env *env, int last_status);

char   *strndup2(const char *s, size_t n);

/* ---- utils ---- */
void   *safe_malloc(size_t size);
size_t  ft_strlen(const char *s);
char   *ft_strdup(const char *s);
char   *ft_strjoin(const char *a, const char *b);
void   *ft_memcpy(void *dst, const void *src, size_t n);
void   *ft_memset(void *dst, int c, size_t n);
char   *ft_strchr(const char *s, int c);
char   *ft_substr(const char *s, unsigned int start, size_t len);
int     ft_itoa_buf(int n, char *buf, size_t size);
int     ft_ltoa_buf(long n, char *buf, size_t size);
int     ft_atoi(const char *s);
int     argv_len(char **argv);
int     ft_isalpha(int c);
int     ft_isalnum(int c);
int     ft_isdigit(int c);
int     ft_isspace(int c);
int     ft_strcmp(const char *a, const char *b);
void    ft_putstr_fd(const char *s, int fd);
void    ft_dprintf1(int fd, const char *fmt_s, const char *arg);
void   *xcalloc(size_t n, size_t sz);
void    argv_push(char ***pargv, const char *w);
void    redir_push(t_redir **lst, t_redir_type type, int fd_from, const char *arg, int quoted);
const char *skip_quote_sentinel(const char *s);
int     equals_ignoring_sentinels(const char *s, const char *lit);

#endif /* MINISHELL_H */
