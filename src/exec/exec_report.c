#include "minishell.h"
#include "exec_int.h"

void    report_and_exit(int saved, const char *used,
                        char **argv, char **child_env)
{
    if (saved == EACCES)
        ft_dprintf1(STDERR_FILENO, "minishell: %s: Permission denied\n", used);
    else if (saved == ENOENT)
        ft_dprintf1(STDERR_FILENO, "minishell: %s: No such file or directory\n", used);
    else if (saved == ENOEXEC)
        ft_dprintf1(STDERR_FILENO, "minishell: %s: Exec format error\n", used);
    else
        perror("minishell");
    if (child_env)
    {
        int i = 0;
        while (child_env[i])
            free(child_env[i++]);
        free(child_env);
    }
    free((char *)used);
    child_exit(saved == ENOENT ? 127 : 126, argv);
}
