NAME = minishell
SRCDIR = ./src/
SRCS = main.c\
		minishell.c\
		init_env.c\
		all_free.c\
		parse1_semicolon.c\
		parse2_env.c\
		parse3_pack.c\
		parse3_packmeta1.c\
		parse3_packmeta2.c\
		parse3_packutil.c\
		parse4_checksyntax.c\
		parse5_exec.c\
		putout_error.c\
		solve_exit.c\
		util.c

OBJDIR = ./obj/
OBJS = $(SRCS:%.c=$(OBJDIR)%.o)
LIBFTDIR = ./libft/
CC = gcc
CFLAGS = -g -O0

all : $(NAME)

$(OBJDIR)%.o : $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME) : $(OBJS)
	# echo @ : $@
	# echo ^ : $^
	$(CC) $(CFLAGS) -I./includes -L $(LIBFTDIR) -o $@ $^ -lft 

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all
