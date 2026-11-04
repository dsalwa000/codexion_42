NAME	= codexion
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread
RM		= rm -f

SRC_DIR	= src
OBJ_DIR	= obj
INC_DIR	= src

SRC_FILES = main.c parser.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -MP -I$(INC_DIR) -c $< -o $@

-include $(DEPS)

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re