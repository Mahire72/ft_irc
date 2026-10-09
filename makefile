# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/29 22:03:48 by mamaimai          #+#    #+#              #
#    Updated: 2026/10/09 12:33:21 by mamaimai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/main.cpp \
      src/server/Server.cpp \
	  src/handle_command/handle_nick.cpp \
	  src/handle_command/handle_user.cpp \
	  src/handle_command/handle_topic.cpp \
	  src/handle_command/handle_kick.cpp \
	  src/handle_command/handle_join.cpp \
	  src/handle_command/handle_privmsg.cpp \
	  src/handle_command/handle_invite.cpp \
	  src/handle_command/handle_mode.cpp \
      src/server/ServerNetwork.cpp \
      src/server/ServerPoll.cpp \
	  src/server/signal.cpp \
	  src/user/User.cpp \
	  src/channel/Channel.cpp \
	  src/parser/Parser.cpp


OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
