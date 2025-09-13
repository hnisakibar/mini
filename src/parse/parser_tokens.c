#include "minishell.h"
#include "parser_int.h"

const char *tok_repr(t_toktype t)
{
    if (t == T_PIPE)
        return ("|");
    if (t == T_LT)
        return ("<");
    if (t == T_GT)
        return (">");
    if (t == T_DGT)
        return (">>");
    if (t == T_HEREDOC)
        return ("<<");
    return ("word");
}

t_redir_type redir_kind(t_toktype t)
{
    if (t == T_LT)
        return (R_IN);
    if (t == T_GT)
        return (R_OUT_TRUNC);
    if (t == T_DGT)
        return (R_OUT_APPEND);
    return (R_HEREDOC);
}
