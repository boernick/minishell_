NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

SRCS = srcs/builtins/builtins.c\
srcs/env/env.c\
srcs/env/env_var_tokenize.c\
srcs/error/error.c\
srcs/exec/exec.c\
srcs/exec/ex_builtin.c\
srcs/exec/ex_env.c\
srcs/main/minishell.c\
srcs/parse/parse.c\
srcs/signals/signals.c\
srcs/start_program/handle_struct.c\
srcs/start_program/start_program.c\
srcs/tokenize/tokenize.c\
srcs/tokenize/tokenize_ext.c\
srcs/tokenize/utils_tokenize.c\
srcs/utils/utils.c\

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
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@

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
