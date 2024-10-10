NAME	:= minishell

LIBNAME = libft.a
LIBDIR = ./libft
LIBFT = ./libft/libft.a

#------------------------------------------------#
#   INGREDIENTS                                  #
#------------------------------------------------#

HEADERS 	:= ./include
OBJDIR		:= ./obj
SRC     	:= main.c  utils.c utils2.c error.c lexer.c builtins.c init.c env.c
				
OBJS     	:= $(addprefix $(OBJDIR)/,$(SRC:.c=.o))

CC      	:= cc
CFLAGS  	:= -Wall -Wextra -Werror -g -fPIC

#------------------------------------------------#
#   RECIPES                                      #
#------------------------------------------------#
.SILENT:

all: $(NAME)

$(OBJDIR)/%.o: %.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $< -I$(HEADERS)

runlibft:
	$(MAKE) -C $(LIBDIR)


$(NAME): runlibft $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME) 

clean:
	rm -f $(OBJS)
	make -C $(LIBDIR) clean

fclean: clean
	rm -rf $(OBJDIR)
	rm -f $(NAME)
	make -C $(LIBDIR) fclean


re: fclean all

.PHONY: all clean fclean re