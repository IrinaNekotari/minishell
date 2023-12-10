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

NAME 	= minishell
AUTHOR	= nmascrier & mjuette
FLAG	= -Wall -Wextra -Werror -g3

SRC	=	src/main.c \
		src/utils/_io_writer.c \
		src/utils/_io_input.c \
		src/utils/_io_output.c \
		src/utils/work_env.c \
		src/utils/work_env_func.c \
		src/execute/pwd.c \
		src/execute/exit.c \
		src/execute/env.c \
		src/execute/echo.c \
		src/execute/cd.c \
		src/execute/bin.c \
		src/execute/bin_forks.c \
		src/execute/unset.c \
		src/execute/export.c \
		src/chains/depths.c \
		src/chains/lengths.c \
		src/parse_variables.c \
		src/errors.c \
		src/parse.c \
		src/parse_single.c \
		src/memory.c \
		src/logger.c \
		src/quote_utils.c \
		src/char_utils.c \
		src/debug_exec.c \
		src/execute.c \
		src/countersplit.c \
		src/syntax_checker.c \
		src/links_rollbacks.c \
		src/io.c \
		src/display.c\
		src/concat.c \
		src/sort_env.c\
		src/builtin.c \
		src/check_pipes.c \
		src/check_pipes2.c \
		src/parse_single_utils.c \
		src/parse_utils.c \
		src/countersplit_utils.c

OBJ  = $(SRC:.c=.o)

#Flags pour les nettoyages
CLEANCMD = rm -rf src/*.o src/execute/*.o src/utils/*.o
FCLEANCMD = rm -f *.a minishell

#Cree la lib, et garde les fichiers .o generes
all:$(NAME)

$(NAME): lib $(OBJ)
	@echo "\e[34mCompilation de minishell ...\e[97m\e[4m"
	@cc $(FLAG) -o $(NAME) $(OBJ) -Iinclude -Llibft -lft -lreadline -g
	@echo "\e[0m\033[1;32mMinishell compilee.\n"

lib:
	@echo "\e[35mCompilation de libft ...\e[97m\e[4m"
	$(MAKE) -C libft
	@echo "\e[0m\033[1;32mLibft compilee.\n"

#compile un unique fichier .c en .o
%.o: %.c
	@cc $(FLAG) -c $< -o $@ -Iinclude

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

.PHONY: all clean fclean re
