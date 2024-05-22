NAME	:= ircserv
CPP		:= c++
#FLAGS	:= -std=c++98 -Wall -Wextra -Werror -fsanitize=address -g
FLAGS	:= -std=c++98 -Wall -Wextra -Werror
SRCDIR	:= src
INCDIR	:= inc
OBJDIR	:= obj
SRC		:= main.cpp \
			Server.cpp \
			serverCommands.cpp \
			serverExceptions.cpp \
			serverPolling.cpp \
			serverUtils.cpp \
			channelOperations.cpp \
			Client.cpp
SRC		:= $(addprefix $(SRCDIR)/, $(SRC))
OBJ		:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
HEADER	:= -I $(INCDIR)

COLOR_GREEN := \033[0;32m
COLOR_YELLOW := \033[0;33m
COLOR_RESET := \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CPP) $(FLAGS) $(HEADER) $^ -o $@
	@echo "$(COLOR_GREEN)IRC was successfully compiled!$(COLOR_RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@$(CPP) $(FLAGS) $(HEADER) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@echo "$(COLOR_YELLOW)Removing obj/ directory$(COLOR_RESET)"
	@rm -fr $(OBJDIR)

fclean: clean
	@echo "$(COLOR_YELLOW)Removing $(NAME)$(COLOR_RESET)"
	@rm -f $(EXEC)

re: fclean all

.PHONY: all, clean, fclean, re

