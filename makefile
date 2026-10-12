# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omawele <omawele@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/29 22:03:48 by mamaimai          #+#    #+#              #
#    Updated: 2026/10/12 12:41:08 by omawele          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
BOT_NAME = bot

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/main.cpp \
      src/server/Server.cpp \
	  src/server/handle_nick.cpp\
	  src/server/handle_user.cpp\
	  src/server/handle_topic.cpp\
	  src/server/handle_kick.cpp\
	  src/server/handle_join.cpp\
	  src/server/handle_privmsg.cpp\
	  src/server/handle_invite.cpp\
	  src/server/handle_mode.cpp\
      src/server/ServerNetwork.cpp \
      src/server/ServerPoll.cpp \
	  src/user/User.cpp \
	  src/channel/Channel.cpp \
	  src/parser/Parser.cpp
	  
BOT_SRC = src/bot/bot.cpp

OBJ = $(SRC:.cpp=.o)
BOT_OBJ = $(BOT_SRC:.cpp=.o)

all: $(NAME)

bonus: $(BOT_NAME) 

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(BOT_NAME): $(BOT_OBJ)
	$(CXX) $(CXXFLAGS) $(BOT_OBJ) -o $(BOT_NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

clean:
	rm -f $(OBJ) $(BOT_OBJ)

fclean: clean
	rm -f $(NAME) $(BOT_NAME)

re: fclean all
