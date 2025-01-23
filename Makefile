NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-g flag for debugger info
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

SRCS =	srcs/builtins/builtins.c\
srcs/builtins/builtins2.c\
srcs/builtins/builtins_utils1.c\
srcs/builtins/builtins_utils2.c\
srcs/builtins/builtins_utils3.c\
srcs/env/env.c\
srcs/env/env2.c\
srcs/env/env_var_tokenize.c\
srcs/env/env_var_tokenize2.c\
srcs/error/error.c\
srcs/error/error2.c\
srcs/error/error3.c\
srcs/exec/exec.c\
srcs/exec/exec2.c\
srcs/exec/ex_builtin.c\
srcs/exec/ex_env.c\
srcs/exec/ex_heredoc.c\
srcs/heredoc/heredoc.c\
srcs/heredoc/heredoc_env_var.c\
srcs/main/cleanup.c\
srcs/main/minishell.c\
srcs/main/run_mini.c\
srcs/parse/conv_lex_to_parse.c\
srcs/parse/conv_lex_to_parse2.c\
srcs/parse/parse.c\
srcs/signals/signals.c\
srcs/signals/singals2.c\
srcs/tokenize/classify_token_types.c\
srcs/tokenize/handle_struct.c\
srcs/tokenize/split_tokens.c\
srcs/tokenize/split_tokens1.c\
srcs/tokenize/tokenize.c\
srcs/tokenize/utils_tokenize.c\
srcs/tokenize/validate_token.c\
srcs/utils/trim_token_quotes.c\
srcs/utils/utils.c\
srcs/utils/utils2.c\

OBJS = $(SRCS:.c=.o)
HEADER = ../../includes/minishell.h

PRINTF_DIR = includes/ft_printf
PRINTF = $(PRINTF_DIR)/libftprintf.a
LIBFT_DIR = includes/Libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I $(PRINTF_DIR) -I $(LIBFT_DIR) -I includes

all: $(NAME)

$(NAME): $(OBJS) $(PRINTF) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(PRINTF) $(LIBFT) $(LDFLAGS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES)-c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR) bonus

$(PRINTF): $(LIBFT)
	make -C $(PRINTF_DIR)

clean:
	rm -f $(OBJS)
	make -C $(PRINTF_DIR) clean
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(PRINTF_DIR) fclean
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
