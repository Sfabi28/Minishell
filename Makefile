CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
COMP_FLAGS = -lft -lreadline

NAME = minishell

SRCS = src/env_utils.c src/signal.c src/execve.c src/f_strjoin.c src/handle_quote.c \
		src/heredoc.c src/line_utils.c src/main.c src/memory_utils.c src/minishell.c \
		src/utils2.c src/pipe_shell.c src/pipex_bonus.c	src/pipex_utils.c src/utils.c \
		src/set_line.c src/set_lst_new.c src/shell_utils.c src/shell.c src/shell2.c \
		src/awk_utils.c src/wildcards.c src/shell3.c src/set_pipe_line.c

all: $(NAME)

$(NAME):
	@make re -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) -I $(LIBFT_DIR) -L $(LIBFT_DIR) $(SRCS) -o $(NAME) $(COMP_FLAGS)

clean:
	make clean -C $(LIBFT_DIR)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)

re: fclean all

fast: re
	make clean -C $(LIBFT_DIR)
	clear

.PHONY: all clean fclean re fast
