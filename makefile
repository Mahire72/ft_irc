# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/29 22:03:48 by mamaimai          #+#    #+#              #
#    Updated: 2026/09/07 14:12:51 by mayoucha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/main.cpp \
      src/server/Server.cpp \
      src/server/ServerNetwork.cpp \
      src/server/ServerPoll.cpp \
	  src/user/User.cpp \
	  src/channel/Channel.cpp


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
