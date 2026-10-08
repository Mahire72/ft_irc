# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/29 22:03:48 by mamaimai          #+#    #+#              #
#    Updated: 2026/10/08 18:25:43 by mayoucha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

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
