#ifndef CORE_H
# define CORE_H

# include "ast.h"
typedef struct s_ctx t_ctx;

void    free_all(t_ctx *ctx);
int     handle_parent_exit(t_pipeline *pl, t_ctx *ctx, char **envp);
int     is_interactive(void);
char    *read_one_line(void);

#endif
