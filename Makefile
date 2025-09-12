NAME     := minishell
CC       := cc
CFLAGS   := -Wall -Wextra -Werror
INCS     := -Iinclude
SRCDIR   := src
OBJDIR   := obj

SRCS := $(SRCDIR)/core/main.c \
        $(SRCDIR)/core/input.c \
        $(SRCDIR)/core/fast_exit.c \
        $(SRCDIR)/core/or_chain.c \
        $(SRCDIR)/signals.c \
        $(SRCDIR)/exec/child_exit.c \
        $(SRCDIR)/exec/exec_path.c \
        $(SRCDIR)/exec/exec_resolve.c \
        $(SRCDIR)/exec/exec_report.c \
        $(SRCDIR)/exec/exec_run.c \
        $(SRCDIR)/exec/exec_run2.c \
        $(SRCDIR)/exec/exec_pipes.c \
        $(SRCDIR)/builtins/dispatch.c \
        $(SRCDIR)/builtins/echo.c \
        $(SRCDIR)/builtins/pwd.c \
        $(SRCDIR)/builtins/env.c \
        $(SRCDIR)/builtins/exit2.c \
        $(SRCDIR)/exec/exec_redir.c \
        $(SRCDIR)/exec/exec_redir2.c \
        $(SRCDIR)/exec/exec_redir3.c \
        $(SRCDIR)/env/env.c \
        $(SRCDIR)/env/env2.c \
        $(SRCDIR)/builtins/cd.c \
        $(SRCDIR)/builtins/cd2.c \
        $(SRCDIR)/builtins/export.c \
        $(SRCDIR)/builtins/export2.c \
        $(SRCDIR)/builtins/unset.c \
        $(SRCDIR)/builtins/exit.c \
        $(SRCDIR)/parse/expand.c \
        $(SRCDIR)/parse/expand2.c \
        $(SRCDIR)/parse/expand3.c \
        $(SRCDIR)/core/resources.c \
        $(SRCDIR)/utils/utils.c \
        $(SRCDIR)/utils/str_utils.c \
        $(SRCDIR)/utils/str_utils2.c \
        $(SRCDIR)/utils/num_utils.c \
        $(SRCDIR)/utils/char_utils.c \
        $(SRCDIR)/utils/argv_utils.c \
        $(SRCDIR)/utils/quote_utils.c \
        $(SRCDIR)/parse/lex_utils.c \
        $(SRCDIR)/parse/lex_ops.c \
        $(SRCDIR)/parse/lex_quote.c \
        $(SRCDIR)/parse/lex_word.c \
        $(SRCDIR)/parse/lex_line.c \
        $(SRCDIR)/parse/parser_core.c \
        $(SRCDIR)/parse/parser_tokens.c \
        $(SRCDIR)/parse/parser_nodes.c \
        $(SRCDIR)/parse/parser_actions.c \
        $(SRCDIR)/parse/parser_api.c \
        $(SRCDIR)/parse/parser_utils.c \
        $(SRCDIR)/exec/exec_child.c \
        $(SRCDIR)/exec/exec_pipe_helpers.c \
        $(SRCDIR)/exec/exec_single_builtin.c \
        $(SRCDIR)/exec/exec_wait.c \

OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(INCS) -lreadline -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
.PHONY: all clean fclean re
