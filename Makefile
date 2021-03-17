NAME = minishell
SRCDIR = ./src/
SRCNAMES = $(shell ls $(SRCDIR) | grep .c$)
SRCS = $(addprefix $(SRCDIR), $(SRCNAMES))
OBJDIR = ./obj/
OBJS = $(SRCNAMES:%.c=$(OBJDIR)%.o)

LIBDIR = ./libft/

CC = gcc
CFLAGS = -g -O0

all : $(NAME)

dir_obj :
	mkdir -p obj

$(OBJDIR)%.o : $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -I./includes -L $(LIBDIR) -o $@ $^ -lft

clean :
	make clean -C $(LIBDIR)
	rm -f $(OBJS)

fclean : clean
	make fclean -C $(LIBDIR)
	rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re
