# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/12 15:36:14 by mwallage          #+#    #+#              #
#    Updated: 2024/05/18 17:16:34 by mwallage         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP		:= c++
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
			Client.cpp
SRC		:= $(addprefix $(SRCDIR)/, $(SRC))
OBJ		:= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
HPP		:= Server.hpp \
			Client.hpp
HPP		:= $(addprefix $(INCDIR)/, $(HPP))
HEADER	:= -I$(INCDIR)
EXEC	:= ircserv

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CPP) $(FLAGS) $^ -o $@

%.o: %.cpp $(HPP)
	$(CPP) $(FLAGS) $(HEADER) -c $< -o $@ 

clean:
	rm -f *.o

fclean: clean
	rm -f $(EXEC)

re: fclean
	make

.PHONY: all, clean, fclean, re

