NAME = libftprintf.a
CC = cc
CFLAGS = -Wall -Wextra -Werror 
SRCS = ft_printf.c ft_printnum.c ft_printstr.c ft_printp.c
OBJS = $(SRCS:.c=.o)
HEADER = ft_printf.h

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	ar rcs $(NAME) $(OBJS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
