NAME	:= ircserv
CPP		:= c++
FLAGS	:= -std=c++98 -Wall -Wextra -Werror
#FLAGS	:= -std=c++98 -Wall -Wextra -Werror
HOSTNAME ?=	$(shell hostname)
SRCDIR	:= src
INCDIR	:= inc
OBJDIR	:= obj
SRC		:= main.cpp \
			Channel.cpp \
			ChannelChecks.cpp \
			ChannelModes.cpp \
			channelOperations.cpp \
			Client.cpp \
			ClientGetSet.cpp \
			ConfigParser.cpp \
			joinChannel.cpp \
			removeClient.cpp \
			Server.cpp \
			ServerBuffer.cpp \
			ServerCommands.cpp \
			ServerExceptions.cpp \
			ServerFind.cpp \
			ServerPolling.cpp \
			ServerUtils.cpp
SRC		:= $(addprefix $(SRCDIR)/, $(SRC))
OBJ		:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
HEADER	:= -I $(INCDIR)

BOT			:= botbot
BOT_SRCDIR	:= chatbot/src
BOT_INCDIR	:= chatbot/inc
BOT_OBJDIR	:= chatbot/obj
BOT_SRC     := bot.cpp \
				main.cpp
BOT_SRC		:= $(addprefix $(BOT_SRCDIR)/, $(BOT_SRC))
BOT_OBJ		:= $(patsubst $(BOT_SRCDIR)/%.cpp, $(BOT_OBJDIR)/%.o, $(BOT_SRC))
BOT_HEADER	:= -I $(BOT_INCDIR)


COLOR_GREEN := \033[0;32m
COLOR_YELLOW := \033[0;33m
COLOR_RESET := \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CPP) $(FLAGS) $(HEADER) $^ -o $@
	@echo "$(COLOR_GREEN)IRC was successfully compiled!$(COLOR_RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@$(CPP) $(FLAGS) $(HEADER) -DHOSTNAME=\"$(HOSTNAME)\" -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

log:
	@git log -n 10 --pretty=format:"%C(yellow)Branch: %D%n%C(red)Author: %an%n%C(green)Date: %ad%n%C(cyan)Commit ID: %H%n%C(magenta)Comment: %s%n%C(reset)%n" --date=format:"%d/%m/%Y à %H:%M"

clean:
	@echo "$(COLOR_YELLOW)Removing obj/ directory$(COLOR_RESET)"
	@rm -fr $(OBJDIR)
	@echo "$(COLOR_YELLOW)Removing chatbot/obj/ directory$(COLOR_RESET)"
	@rm -fr $(BOT_OBJDIR)

fclean: clean
	@echo "$(COLOR_YELLOW)Removing $(NAME)$(COLOR_RESET)"
	@rm -f $(NAME)
	@echo "$(COLOR_YELLOW)Removing $(BOT)$(COLOR_RESET)"
	@rm -f $(BOT)

$(BOT): $(BOT_OBJ)
	@$(CPP) $(FLAGS) $(BOT_HEADER) $^ -o $@
	@echo "$(COLOR_GREEN)Botbot was successfully assembled!$(COLOR_RESET)"

$(BOT_OBJDIR)/%.o: $(BOT_SRCDIR)/%.cpp | $(BOT_OBJDIR)
	@$(CPP) $(FLAGS) $(BOT_HEADER) -D HOSTNAME=$(HOSTNAME) -c $< -o $@

$(BOT_OBJDIR):
	@mkdir -p $(BOT_OBJDIR)


re: fclean all

.PHONY: all, clean, fclean, re

