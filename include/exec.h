#ifndef EXEC_H
# define EXEC_H

# include "ast.h"
typedef struct s_ctx t_ctx;

int     execute_pipeline(const t_pipeline *p, t_ctx *ctx, char **envp);

#endif

