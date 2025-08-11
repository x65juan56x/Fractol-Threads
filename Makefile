
# Project name and compiler flags

NAME     := fractol
CC       := cc
CFLAGS   := -Wextra -Wall -Werror -Ofast


# Paths to headers and libraries

INC_DIRS := -I include \
			-I libraries/MLX42/include \
			-I libraries/libft \
			-I libraries/ft_printf
MLX_DIR  := libraries/MLX42
FTPF_DIR := libraries/ft_printf

# static libs (full paths)
MLX_LIB  := $(MLX_DIR)/build/libmlx42.a
FTPF_LIB := $(FTPF_DIR)/libftprintf.a

# link flags
LDLIBS   := $(MLX_LIB) \
			$(FTPF_LIB) \
			-ldl -lglfw -pthread -lm


# Source files

SRC_DIR     := src
LIBFT_DIR   := libraries/libft

SRC_COMMON  := $(shell find $(SRC_DIR) -name "*.c")
SRC_LIBFT   := $(shell find $(LIBFT_DIR) -name "*.c")

OBJ_COMMON  := $(SRC_COMMON:.c=.o)
OBJ_LIBFT   := $(SRC_LIBFT:.c=.o)


# Phony targets

.PHONY: all mlx ft_printf clean fclean re


# Default build: MLX, ft_printf, then the program

all: mlx ft_printf $(NAME)


# Build MLX42 via CMake once

mlx:
	@cmake $(MLX_DIR) -B $(MLX_DIR)/build >/dev/null
	@cmake --build $(MLX_DIR)/build -- -j4 >/dev/null


# Build ft_printf library

ft_printf:
	@$(MAKE) -C $(FTPF_DIR)


# Link main program (no bonus)

$(NAME): $(OBJ_COMMON) $(OBJ_LIBFT)
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(INC_DIRS) -o $@


# Generic rule for compiling .c -> .o

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INC_DIRS) -o $@


# Remove all object files and ft_printf objects

clean:
	rm -f $(OBJ_COMMON) $(OBJ_LIBFT)
	@$(MAKE) -C $(FTPF_DIR) clean


# Remove everything including binaries and ft_printf lib

fclean: clean
	rm -f $(NAME) $(FTPF_LIB)


# Rebuild from scratch

re: fclean all