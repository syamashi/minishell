NAME = minishell
SRCDIR = ./src/
SRCS = main.c\
		minishell.c\
		env_init.c\
		all_free.c\
		parsenew1_pack.c\
		parsenew1_packmeta1.c\
		parsenew1_packmeta2.c\
		parsenew1_util.c\
		parsenew2_syntaxcheck.c\
		parsenew3_storediv.c\
		parsenew4_envexpand.c\
		parsenew5_trim.c\
		parse5_exec.c\
		parsenew6_ast.c\
		parsenew7_exinit.c\
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
