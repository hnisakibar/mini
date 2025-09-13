#include "minishell.h"
#include "exec_int.h"

void    exit_msg(int code, const char *fmt, const char *arg, char **argv)
{
    ft_dprintf1(STDERR_FILENO, fmt, arg);
    child_exit(code, argv);
}

char    *resolve_direct(const char *arg0, char **argv)
{
    struct stat st;

    if (stat(arg0, &st) != 0)
        exit_msg(127, "minishell: %s: No such file or directory\n", arg0, argv);
    if (S_ISDIR(st.st_mode))
        exit_msg(126, "minishell: %s: Is a directory\n", arg0, argv);
    if (access(arg0, X_OK) != 0)
        exit_msg(126, "minishell: %s: Permission denied\n", arg0, argv);
    return (ft_strdup(arg0));
}

char    *resolve_via_path(const char *arg0, char **argv, t_env *env)
{
    char    *cmd_path;

    if (ft_strcmp(arg0, ".") == 0 || ft_strcmp(arg0, "..") == 0)
        exit_msg(127, "minishell: %s: command not found\n", arg0, argv);
    cmd_path = find_in_path(arg0, env);
    if (!cmd_path)
        exit_msg(127, "minishell: %s: command not found\n", arg0, argv);
    return (cmd_path);
}

char    *resolve_cmd(char **argv, t_env *env)
{
    if (!argv || !argv[0] || !*argv[0])
        return (NULL);
    if (strchr(argv[0], '/'))
        return (resolve_direct(argv[0], argv));
    return (resolve_via_path(argv[0], argv, env));
}
