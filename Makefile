NAME = minishell
SRCDIR = ./src/
SRCS = main.c\
		minishell.c\
		env_init.c\
		env_sort.c\
		all_free.c\
		parse1_pack.c\
		parse1_packmeta1.c\
		parse1_packmeta2.c\
		parse1_util.c\
		parse2_inputcheck.c\
		parse2_inputcheck_syntax.c\
		parse3_storediv.c\
		parse4_envexpand.c\
		parse4_envexpand2_envjoin.c\
		parse4_envexpand3_repack.c\
		parse5_trim.c\
		parse5_trim2_quotedel.c\
		parse6_ast.c\
		parse7_exinit.c\
		parse7_exinit2_fdcontrol.c\
		parse7_exinit3_pathmake.c\
		putout_error.c\
		util.c\
		debug.c\
		sh_launch.c\
		sh_launch_utils.c\
		ft_execvpe.c\
		sh_prompt.c\
		sh_echo.c\
		sh_export.c\
		sh_export2_no_args.c\
		sh_export3_any_args.c\
		sh_env.c\
		sh_unset.c\
		sh_cd.c\
		sh_pwd.c\
		sh_exit.c

OBJDIR = ./obj/
OBJS = $(SRCS:%.c=$(OBJDIR)%.o)
LIBFTDIR = ./libft/
CC = gcc
CFLAGS = -g -O0

all : $(NAME)

$(OBJDIR)%.o : $(SRCDIR)%.c dir_obj 
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME) : $(OBJS)
	# echo @ : $@
	# echo ^ : $^
	$(CC) $(CFLAGS) -I./includes -L $(LIBFTDIR) -o $@ $^ -lft

:
	mkdir -p obj

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all
