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

NAME = libft.a

SRC = ft_memset.c ft_bzero.c ft_calloc.c ft_memcpy.c ft_memmove.c ft_memchr.c ft_memcmp.c ft_strlen.c ft_strdup.c ft_strlcat.c ft_strlcpy.c ft_strchr.c ft_strrchr.c ft_strnstr.c ft_strncmp.c ft_atoi.c ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c ft_strmapi.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_printf.c ft_util.c ft_lstnew.c ft_lstdelone.c ft_lstclear.c ft_lstadd_front.c ft_lstadd_back.c ft_lstsize.c ft_lstiter.c ft_lstmap.c ft_lstlast.c get_next_line.c get_next_line_utils.c ft_abs.c

OBJ  = $(SRC:.c=.o)

#Flags pour les nettoyages
CLEANCMD = rm -rf *.o
FCLEANCMD = rm -f *.a

#Cree la lib, et garde les fichiers .o generes
all:$(NAME) clean

$(NAME):$(OBJ)
	@ar rc $(NAME) $(OBJ)

#compile un unique fichier .c en .o
%.o: %.c
	@gcc $(FLAG) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@echo ".o deleted"

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) deleted"

re: fclean all
