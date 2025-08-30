NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=thread -pthread

SRC = main.c \
      philo_init.c \
      philo_monitor.c \
      philo_routine.c \
      philo_utils_extra.c \
      philo_utils.c \
      philo_clean.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(YELLOW)🔗 Linking object files...$(NC)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "$(GREEN)✅ $(NAME) compiled successfully!$(NC)"

%.o: %.c
	@echo "$(BLUE)🔨 Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)🧹 Cleaning object files...$(NC)"
	@rm -rf $(OBJ)
	@echo "$(GREEN)✅ Object files cleaned!$(NC)"

fclean: clean
	@echo "$(YELLOW)🔥 Removing executable...$(NC)"
	@rm -rf $(NAME)
	@echo "$(GREEN)✅ Full clean completed!$(NC)"

re: fclean all

.PHONY: all clean fclean re