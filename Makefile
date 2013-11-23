##
## Makefile for Pamela in /home/jack/workspace/crypto/pamela
## 
## Made by Jack
## Login   <jack@epitech.net>
## 
## Started on  Thu Oct 31 13:31:42 2013 Jack
## Last update Sat Nov 23 02:04:33 2013 Jack
##

NAME		=	pamela_mount.so

GCC		=	gcc
LD		=	ld
RM		=	rm -f

CFLAGS		+=	-Wall -Wextra -Werror
CFLAGS		+=	-ansi
CFLAGS		+=	$(DFLAGS) $(INCLUDES)
CFLAGS		+=	-fPIC

DFLAGS		=	-D _BSD_SOURCE

LIB		=	-lxml2 -lz -lm -ldl -lcryptsetup

INCLUDES	=	-I /usr/include/libxml2

DEBUG		=	-g3

SRCDIR		=	srcs/

SRCS		=	$(SRCDIR)pamela.c		\
			$(SRCDIR)log.c			\
			$(SRCDIR)rd_config.c		\
			$(SRCDIR)rd_user_config.c	\
			$(SRCDIR)loop.c			\
			$(SRCDIR)crypt.c		\
			$(SRCDIR)mount.c

OBJS		=	$(SRCS:%.c=%.o)

all:			$(NAME)

$(NAME):		$(OBJS)
			$(LD) -x --shared -o $(NAME) $(OBJS) $(LIB)

clean:
			$(RM) $(OBJS)

fclean:			clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:			all clean fclean re
