/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_int.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekibar <ekibar@student.42istanbul.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:40:00 by ekibar            #+#    #+#             */
/*   Updated: 2025/09/11 00:15:00 by ekibar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INT_H
# define PARSER_INT_H

# include <stddef.h>
# include "ast.h"

/* small helper pair for current/last command */
typedef struct s_cmdpair
{
    t_cmd   *cur;
    t_cmd   *last;
    int      pending_fd; /* numeric fd prefix before a redirection, or -1 */
}   t_cmdpair;

/* token helpers */
const char  *tok_repr(t_toktype t);
t_redir_type redir_kind(t_toktype t);

/* command/pipeline setup */
t_cmd       *cmd_new(void);
void        attach_cmd(t_pipeline *pl, t_cmdpair *cp);

/* actions */
int         ensure_pipe_ok(t_cmd *cur);
int         handle_redir(t_tokarr *ta, size_t *i, t_cmd *cur, int *pending_fd);
int         handle_token(t_tokarr *ta, size_t *i,
                        t_pipeline *pl, t_cmdpair *cp);

#endif /* PARSER_INT_H */
