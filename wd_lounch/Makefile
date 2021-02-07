# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ewatanab <ewatanab@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/02 15:54:58 by ewatanab          #+#    #+#              #
#    Updated: 2021/02/02 16:10:42 by ewatanab         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= lounch
SRC		= ./sh_lounch.c \
		  ./sh_lounch_utils.c \
		  ./ft_execvpe.c \
		  ./exec_test.c
OBJ		= $(SRC:%.c=%.o)
LIBFTDIR= ../libft/
CC		= gcc
CFLAGS	= -g -O0

all: $(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C $(LIBFTDIR)
	$(CC) $^ -L $(LIBFTDIR) -lft -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
