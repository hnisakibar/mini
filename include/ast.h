/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekibar <ekibar@student.42istanbul.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 08:00:00 by ekibar            #+#    #+#             */
/*   Updated: 2025/09/11 00:10:00 by ekibar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <stddef.h>
# include <stdbool.h>

/* Redirections */
typedef enum e_redir_type
{
    R_IN,
    R_OUT_TRUNC,
    R_OUT_APPEND,
    R_HEREDOC
}   t_redir_type;

typedef struct s_redir
{
    t_redir_type    type;
    char            *arg;
    struct s_redir  *next;
}   t_redir;

/* Command and pipeline */
typedef struct s_cmd
{
    char            **argv;   /* NULL-terminated */
    t_redir          *redirs; /* linked list */
    struct s_cmd     *next;   /* piped chain */
}   t_cmd;

typedef struct s_pipeline
{
    t_cmd   *head;            /* first command */
}   t_pipeline;

/* Tokens */
typedef enum e_toktype
{
    T_WORD,
    T_PIPE,
    T_LT,
    T_GT,
    T_DGT,
    T_HEREDOC
}   t_toktype;

typedef struct s_tok
{
    t_toktype   type;
    char        *val;   /* only for T_WORD */
}   t_tok;

typedef struct s_tokarr
{
    t_tok   *v;
    size_t   n;
    size_t   cap;
}   t_tokarr;

/* Lexer API */
void    lex_line(const char *s, t_tokarr *out);
void    free_tokens(t_tokarr *ta);

/* Parser API */
int     parse_line(const char *line, t_pipeline **out);
void    free_pipeline(t_pipeline *p);

#endif /* AST_H */
