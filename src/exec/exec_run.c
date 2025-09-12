#include "minishell.h"
#include "exec_int.h"
int run_if_builtin(char **argv, t_ctx *ctx)
{
    if (!argv || !argv[0])
        return (0);
    if (!is_builtin(argv[0]))
        return (-1);
    signal(SIGPIPE, SIG_IGN);
    return run_builtin(argv, ctx);
}
static int count_environ(char **envp)
{
    int n = 0;
    if (!envp) return 0;
    while (envp[n]) n++;
    return n;
}
static int find_underscore(char **envp)
{
    int i = 0;
    while (envp && envp[i])
    {
        if (strncmp(envp[i], "_=", 2) == 0)
            return i;
        i++;
    }
    return -1;
}
static char **make_child_env_with_uscore(t_ctx *ctx, const char *underscore)
{
    char **base = env_to_environ(ctx->env);
    int n = count_environ(base);
    int idx = find_underscore(base);
    if (idx >= 0)
    {
        free(base[idx]);
        base[idx] = make_pair("_", underscore ? underscore : "");
        return base;
    }
    char **arr = (char **)safe_malloc(sizeof(char *) * (n + 2));
    for (int i = 0; i < n; i++) arr[i] = base[i];
    arr[n] = make_pair("_", underscore ? underscore : "");
    arr[n + 1] = NULL;
    free(base);
    return arr;
}
static int should_fallback(int err, const char *cmd_path)
{
	if (err == ENOEXEC)
		return (1);
	if (err == ENOENT && cmd_path)
	{
		struct stat st;
		if (stat(cmd_path, &st) == 0 && S_ISREG(st.st_mode) &&
			access(cmd_path, R_OK) == 0)
			return (1);
	}
	return (0);
}
static void exec_with_sh(char *cmd_path, char **child_env)
{
	char *sh_argv[3];
	sh_argv[0] = (char *)"sh";
	sh_argv[1] = cmd_path ? cmd_path : (char *)"";
	sh_argv[2] = NULL;
	execve("/bin/sh", sh_argv, child_env);
}
void try_exec_or_fallback(char *cmd_path, char **argv, char **child_env)
{
	int saved;

	execve(cmd_path, argv, child_env);
	saved = errno;
	if (should_fallback(saved, cmd_path))
	{
		exec_with_sh(cmd_path, child_env);
		saved = errno;
	}
	{
		char *used = cmd_path ? cmd_path : ft_strdup(argv[0]);
		report_and_exit(saved, used, argv, child_env);
	}
}
static void handle_empty_command(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
	{
		ft_dprintf1(STDERR_FILENO, "minishell: %s: command not found\n", cmd);
		_exit(127);
	}
}
static void reparse_and_run(char **argv, t_ctx *ctx)
{
    char *re_line = join_words(argv);
    t_pipeline *pl = NULL;
    int rc = parse_line(re_line, &pl);
    free(re_line);
    if (rc != 0)
    {
        if (pl)
            free_pipeline(pl);
        child_exit(rc, argv);
    }
    rc = execute_pipeline(pl, ctx, NULL);
    free_pipeline(pl);
    child_exit(rc, argv);
}
int exec_inplace(char **orig_argv, char **argv, t_ctx *ctx, char **envp_unused)
{
    char *cmd_path, **child_env;
    int rc;

	(void)envp_unused;
	if (!argv || !argv[0])
		_exit(0);
	handle_empty_command(argv[0]);
    if (needs_reparse(orig_argv, argv))
        reparse_and_run(argv, ctx);
    rc = run_if_builtin(argv, ctx);
    if (rc >= 0)
        child_exit(rc, argv);
    cmd_path = resolve_cmd(argv, ctx->env);
    child_env = make_child_env_with_uscore(ctx, cmd_path ? cmd_path : argv[0]);
    try_exec_or_fallback(cmd_path, argv, child_env);
    return (0);
}
