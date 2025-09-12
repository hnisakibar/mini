#include "minishell.h"

int	msh_builtin_pwd(void)
{
	char	buf[PATH_MAX];

    if (getcwd(buf, sizeof(buf)) == NULL)
    {
        perror("pwd");
        return (1);
    }
    ft_putstr_fd(buf, STDOUT_FILENO);
    ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}

