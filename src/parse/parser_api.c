#include "minishell.h"



int	parse_line(const char *line, t_pipeline **out)
{
	int		rc;
	t_tokarr	ta;

	*out = NULL;
	lex_line(line, &ta);
	rc = parse_tokens(&ta, out);
	free_tokens(&ta);
	return (rc);
}



static void	free_argv(char **argv)
{
    size_t i = 0;
    if (!argv)
        return;
    while (argv[i])
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}

static void	free_redirs(t_redir *r)
{
    t_redir *tmp;
    while (r)
    {
        tmp = r->next;
        if (r->arg)
            free(r->arg);
        free(r);
        r = tmp;
    }
}

static void	free_cmds(t_cmd *cmd)
{
    t_cmd *tmp;
    while (cmd)
    {
        tmp = cmd->next;
        free_argv(cmd->argv);
        free_redirs(cmd->redirs);
        free(cmd);
        cmd = tmp;
    }
}

void	free_pipeline(t_pipeline *p)
{
    if (!p)
        return;
    free_cmds(p->head);
    free(p);
}
