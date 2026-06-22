# -------------------------------- VARIABLES --------------------------------- #

NAME		=	Get_Next_Line
TEST		=	test

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g

RM			=	rm -f

HFILES		=	get_next_line.h

SRCS		=	\
				main.c \
				get_next_line.c \
				get_next_line_utils.c

OBJS 		=	$(SRCS:.c=.o)

# ---------------------------------- RULES ----------------------------------- #

all: $(NAME)

$(NAME): $(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HFILES)
			$(CC) $(CFLAGS) -c $< -o $@

dgdb: $(OBJS)
			gcc $(CFLAGS) $(OBJS) -o $(TEST)

dlldb: $(OBJS)
			clang $(CFLAGS) $(OBJS) -o $(TEST)

clean:
			$(RM) $(OBJS)

fclean: clean
			$(RM) $(NAME) $(TEST)

re: fclean all

.PHONY: all clean fclean re dgdb dlldb
