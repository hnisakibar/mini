/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_int.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekibar <ekibar@student.42istanbul.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:40:00 by ekibar            #+#    #+#             */
/*   Updated: 2025/09/11 00:15:00 by ekibar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_INT_H
# define EXEC_INT_H

# include <stddef.h>
# include <sys/types.h>

/* forward decls to avoid heavy includes */
typedef struct s_env t_env;
typedef struct s_ctx t_ctx;

/* cleanup helpers in child */
void    free_strv(char **v);
void    child_exit(int rc, char **argv);

/* path helpers */
int     is_executable(const char *p);
char   *join3(const char *a, const char *b, const char *c);
char   *scan_path_dirs(char *paths, const char *cmd);
char   *find_in_path(const char *cmd, t_env *env);

/* resolve helpers */
void    exit_msg(int code, const char *fmt, const char *arg, char **argv);
char   *resolve_direct(const char *arg0, char **argv);
char   *resolve_via_path(const char *arg0, char **argv, t_env *env);
char   *resolve_cmd(char **argv, t_env *env);

/* exec helpers */
void    report_and_exit(int saved, const char *used,
                        char **argv, char **child_env);
char  **make_child_env(t_ctx *ctx);
void    try_exec_or_fallback(char *cmd_path, char **argv, char **child_env);
int     run_if_builtin(char **argv, t_ctx *ctx);

/* reparse helpers (shared across exec_*.c) */
int     is_meta_only(const char *s);
int     has_sentinel(const char *s);
int     has_dollar(const char *s);
int     needs_reparse(char **orig, char **argv);
size_t  calc_join_len(char **argv);
char   *join_words(char **argv);

#endif /* EXEC_INT_H */
