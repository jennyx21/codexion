NAME     := codexion
CC       := gcc
AR       := ar
ARFLAGS  := rcs
# DEBUGFLAGS := -g
CFLAGS   := -Wall -Wextra -Werror -lpthread $(DEBUGFLAGS)
OBJDIR   := obj

SRC =	codexion.c\
		src/error_messages.c\
		src/parse.c\
		src/waiter.c\
		src/timing.c\
		src/rutine.c\
		src/requests.c\
		src/monitor.c\
		src/dongles.c\
		src/create.c\
		src/actions.c\
		src/read_values.c

OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
all: $(NAME)

# $(OBJDIR)/%.o: %.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re