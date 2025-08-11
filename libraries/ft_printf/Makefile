SRCS  =	ft_printf.c					\
		ft_printf_nums_utils.c		\
		ft_printf_text_utils.c		\
		print_chars.c				\
		print_hex.c					\
		print_nums.c				\


NAME = libftprintf.a

CC = gcc

CC_FLAGS = -Werror -Wall -Wextra

OBJS = $(SRCS:.c=.o)

OBJS_BONUS = $(SRCS:.c=.o)

AR = ar rcs

%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(AR) $@ $?
	@echo "$(NAME), created!"

bonus: $(OBJS)
	$(AR) $(NAME) $?
	@echo "$(NAME), created!"

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re