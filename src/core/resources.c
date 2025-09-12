#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->val)
			free(env->val);
		free(env);
		env = tmp;
	}
}

void	free_all(t_ctx *ctx)
{
	if (ctx && ctx->env)
		free_env(ctx->env), ctx->env = NULL;
	clear_history();
}
