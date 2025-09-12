/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekibar <ekibar@student.42istanbul.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:40:00 by ekibar            #+#    #+#             */
/*   Updated: 2025/09/11 00:15:00 by ekibar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INT_H
# define LEXER_INT_H

# include <stddef.h>
# include "ast.h"

void        *lxr_xrealloc(void *ptr, size_t n);
void         tok_push(t_tokarr *ta, t_tok t);
const char  *emit_operator(const char *s, t_tokarr *ta);
const char  *read_quoted(const char *s, char q, size_t *len, char **buf);
const char  *read_word(const char *s, t_tokarr *ta);

#endif /* LEXER_INT_H */
