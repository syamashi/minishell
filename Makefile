NAME = minishell
SRCDIR = ./src/
SRCS = main.c\
		all_free.c\
		debug.c\
		div_commands.c\
		env_init.c\
		env_sort.c\
		minishell.c\
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
		putout_error2.c\
		ft_execvpe.c\
		sh_cd_utils.c\
		sh_cd.c\
		sh_cd2_linkdel.c\
		sh_echo.c\
		sh_env.c\
		sh_exit.c\
		sh_export.c\
		sh_export2_no_args.c\
		sh_export3_any_args.c\
		sh_launch_utils.c\
		sh_launch.c\
		sh_prompt.c\
		sh_pwd.c\
		sh_unset.c\
		to_ex_list.c\
		util_commandlst.c\
		util_del.c\
		util_env.c\
		util_is.c\
		util_is2.c\
		util_is3.c\
		util_pwdlist1.c\
		util_pwdlist2.c\
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
	$(CC) $(CFLAGS) -I./includes -L $(LIBFTDIR) -o $@ $^ -lft

dir_obj :
	mkdir -p obj

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all
