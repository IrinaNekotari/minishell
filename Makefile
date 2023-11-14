# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/14 12:35:47 by nmascrie          #+#    #+#              #
#    Updated: 2023/04/14 12:35:49 by nmascrie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAG = -Wall -Wextra -Werror

NAME = libftprintf.a

SRC = src/main.c src/parse.c src/parse_error.c src/parse_single.c src/parse_single_2.c src/memory.c src/logger.c src/quote_utils.c \
	 src/char_utils.c src/debug_exec.c src/execute.c src/countersplit.c \
	 src/syntax_checker.c src/io.c \
	 src/execute/pwd.c src/execute/exit.c \
	 src/utils/_io_writer.c src/utils/_io_input.c src/utils/_io_output.c

OBJ  = $(SRC:.c=.o)

#Flags pour les nettoyages
CLEANCMD = rm -rf src/*.o src/execute/*.o src/utils/*.o
FCLEANCMD = rm -f *.a minishell

#Cree la lib, et garde les fichiers .o generes
all:$(NAME)

$(NAME):lib mini

mini:$(OBJ)
	@echo "\e[34mCompilation de minishell ...\e[97m\e[4m"
	gcc $(FLAG) $(SRC) -o minishell -Iinclude -Llibft/ -lft -lreadline -g
	@echo "\e[0m\033[1;32mMinishell compilee.\n"

lib:
	@echo "\e[35mCompilation de libft ...\e[97m\e[4m"
	$(MAKE) -C libft
	@echo "\e[0m\033[1;32mLibft compilee.\n"

#compile un unique fichier .c en .o
%.o: %.c
	@gcc $(FLAG) -c $< -o $@ -Iinclude -Llibft/ -lft

clean:
	$(CLEANCMD)
	@echo "\e[91m\e[1m"
	$(MAKE) clean -C libft
	@echo "\e[91m\e[1mFichiers .o nettoyes\n"

fclean: clean
	@echo "\e[91m\e[1m"
	$(FCLEANCMD)
	$(MAKE) fclean -C libft
	@echo "\e[91m\e[1mFichiers crees nettoyes\n"

re: fclean all
