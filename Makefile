NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address -g flag for debugger info
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
LDFLAGS = -lreadline

# Debug flag (set to 1 to enable AddressSanitizer) ./minishell DEBUG=1
DEBUG ?= 0

ifeq ($(DEBUG), 1)
    CFLAGS += -fsanitize=address
endif

SRCS = ./srcs/builtins/builtins.c \
./srcs/builtins/builtins2.c \
./srcs/builtins/builtins_utils1.c \
./srcs/builtins/builtins_utils2.c \
./srcs/builtins/builtins_utils3.c \
./srcs/env/env.c \
./srcs/env/env2.c \
./srcs/env/env_var_tokenize.c \
./srcs/error/error.c \
./srcs/error/error2.c \
./srcs/exec/exec.c \
./srcs/exec/exec2.c \
./srcs/exec/ex_heredoc.c \
./srcs/exec/ex_builtin.c \
./srcs/exec/ex_env.c \
./srcs/main/minishell.c \
./srcs/parse/parse.c \
./srcs/signals/signals.c \
./srcs/tokenize/handle_struct.c \
./srcs/tokenize/split_tokens.c \
./srcs/tokenize/heredoc.c \
./srcs/tokenize/tokenize.c \
./srcs/tokenize/tokenize_ext.c \
./srcs/tokenize/utils_tokenize.c \
./srcs/utils/utils.c \
./srcs/utils/utils2.c \
./srcs/utils/get_next_line.c \

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
