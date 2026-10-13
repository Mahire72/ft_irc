# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/29 22:03:48 by mamaimai          #+#    #+#              #
#    Updated: 2026/10/13 19:51:46 by mayoucha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
BOT_NAME = bot

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/main.cpp \
      src/server/Server.cpp \
      src/handle_command/handle_pass.cpp \
      src/handle_command/handle_nick.cpp \
      src/handle_command/handle_user.cpp \
      src/handle_command/handle_topic.cpp \
      src/handle_command/handle_kick.cpp \
      src/handle_command/handle_join.cpp \
      src/handle_command/handle_privmsg.cpp \
      src/handle_command/handle_invite.cpp \
      src/handle_command/handle_mode.cpp \
      src/handle_command/handle_ping.cpp \
      src/handle_command/handle_pong.cpp \
      src/handle_command/handle_who.cpp \
      src/handle_command/handle_cap.cpp \
      src/server/ServerNetwork.cpp \
      src/server/ServerPoll.cpp \
      src/server/signal.cpp \
      src/user/User.cpp \
      src/channel/Channel.cpp \
      src/parser/Parser.cpp

BOT_SRC = src/bot/bot.cpp

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
BOT_OBJ = $(addprefix $(OBJ_DIR)/, $(BOT_SRC:.cpp=.o))

all: $(NAME)

bonus: $(BOT_NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "[SUCCESS] Compiled $(NAME)"

$(BOT_NAME): $(BOT_OBJ)
	@$(CXX) $(CXXFLAGS) $(BOT_OBJ) -o $(BOT_NAME)
	@echo "[SUCCESS] Compiled $(BOT_NAME)"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME) $(BOT_NAME)

re: fclean all

.PHONY: all bonus clean fclean re
