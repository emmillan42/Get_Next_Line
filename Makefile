# -------------------------------- VARIABLES --------------------------------- #

NAME        = Get_Next_Line
BONUS_NAME  = Get_Next_Line_Bonus

TEST        = test
TEST_BONUS  = test_bonus

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g

RM			=	rm -f

HFILES		=	get_next_line.h

SRCS		=	\
				test.c \
				get_next_line.c \
				get_next_line_utils.c

OBJS 		=	$(SRCS:.c=.o)

BONUS_HFILES = get_next_line_bonus.h

BONUS_SRCS   = \
				test_bonus.c \
				get_next_line_bonus.c \
				get_next_line_utils_bonus.c

BONUS_OBJS   = $(BONUS_SRCS:.c=.o)

# ---------------------------------- RULES ----------------------------------- #

all: $(NAME)

$(NAME): $(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
			$(CC) $(CFLAGS) -c $< -o $@

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(BONUS_NAME)

dgdb: $(OBJS)
			gcc $(CFLAGS) $(OBJS) -o $(TEST)

dlldb: $(OBJS)
			clang $(CFLAGS) $(OBJS) -o $(TEST)

dgdb_bonus: $(BONUS_OBJS)
	gcc $(CFLAGS) $(BONUS_OBJS) -o $(TEST)

dlldb_bonus: $(BONUS_OBJS)
	clang $(CFLAGS) $(BONUS_OBJS) -o $(TEST)

clean:
			$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME) $(TEST)

re: fclean all

bonus_re: fclean bonus

.PHONY: all bonus clean fclean re bonus_re dgdb dlldb dgdb_bonus dlldb_bonus
