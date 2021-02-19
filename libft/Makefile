# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: syamashi <syamashi@student.42.tokyo>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/06/24 21:04:46 by syamashi          #+#    #+#              #
#    Updated: 2021/01/06 14:59:39 by syamashi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRCS = $(shell echo *.c)
OBJS = $(SRCS:%.c=%.o)
HEADER = libft.h
CFLAGS = -Wall -Wextra -Werror
CC = gcc

all : $(NAME)

$(NAME) : $(OBJS)
	ar -rc $(NAME) $(OBJS)

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re bonus