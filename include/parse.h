#ifndef PARSE_H
# define PARSE_H

# include "ast.h"

int     parse_line(const char *line, t_pipeline **out);
void    free_pipeline(t_pipeline *p);
int     parse_tokens(t_tokarr *ta, t_pipeline **out);

#endif

