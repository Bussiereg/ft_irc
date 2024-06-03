NAME	:= ircserv
CPP		:= c++
FLAGS	:= -std=c++98 -Wall -Wextra -Werror -fsanitize=address -g
#FLAGS	:= -std=c++98 -Wall -Wextra -Werror
HOSTNAME :=	$(shell hostname)
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
			joinChannel.cpp \
			Channel.cpp \
			Client.cpp \
			ConfigParser.cpp
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
	@$(CPP) $(FLAGS) $(HEADER) -D HOSTNAME=$(HOSTNAME) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

log:
	@git log -n 10 --pretty=format:"%C(yellow)Branch: %D%n%C(red)Author: %an%n%C(green)Date: %ad%n%C(cyan)Commit ID: %H%n%C(magenta)Comment: %s%n%C(reset)%n" --date=format:"%d/%m/%Y à %H:%M"

clean:
	@echo "$(COLOR_YELLOW)Removing obj/ directory$(COLOR_RESET)"
	@rm -fr $(OBJDIR)

fclean: clean
	@echo "$(COLOR_YELLOW)Removing $(NAME)$(COLOR_RESET)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re

