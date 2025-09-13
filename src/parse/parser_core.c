#include "minishell.h"
#include "parser_int.h"

int	parse_tokens(t_tokarr *ta, t_pipeline **out)
{
	t_pipeline	*pl;
	t_cmdpair	cp;
	size_t		i;
	int			rc;

	pl = xcalloc(1, sizeof(*pl));
	cp.cur = NULL;
	cp.last = NULL;
	cp.pending_fd = -1;
	i = 0;
	while (i < ta->n)
	{
		rc = handle_token(ta, &i, pl, &cp);
		if (rc != 0)
		{
			free_pipeline(pl);
			return (rc);
		}
	}
	*out = pl;
	return (0);
}
