#include "minishell.h"

int	msh_builtin_env(t_env *env)
{
	t_env	*e;

	e = env;
    while (e)
    {
        if (e->val)
        {
            ft_putstr_fd(e->key, STDOUT_FILENO);
            ft_putstr_fd("=", STDOUT_FILENO);
            ft_putstr_fd(e->val, STDOUT_FILENO);
            ft_putstr_fd("\n", STDOUT_FILENO);
        }
        e = e->next;
    }
    return (0);
}
