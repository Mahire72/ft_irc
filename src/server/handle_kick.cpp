/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_kick.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 17:49:35 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/08 13:07:47 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

#include <vector>
#include <iostream>

void Server::sendKickNeedMoreParams(User &sender)
{
    sendError("461", "KICK :Not enough parameters", sender);
}

void Server::sendKickNoSuchChannel(User &sender, const std::string &channelName)
{
    sendError("403", channelName + " :No such channel", sender);
}

void Server::sendKickNotOnChannel(User &sender, const std::string &channelName)
{
    sendError("442", channelName + " :You're not on that channel", sender);
}

void Server::sendKickNotOperator(User &sender, const std::string &channelName)
{
    sendError("482", channelName + " :You're not channel operator", sender);
}

void Server::sendKickUserNotInChannel(User &sender,
                                     const std::string &nickname,
                                     const std::string &channelName)
{
    sendError("441",
             nickname + " " + channelName +
             " :They aren't on that channel",
             sender);
}

void Server::sendKickNoSuchNick(User &sender, const std::string &nickname)
{
    sendError("401", nickname + " :No such nick", sender);
}

Channel *Server::findChannel(const std::string &channelName)
{
    std::map<std::string, Channel>::iterator it =
        _channels.find(channelName);
    if (it == _channels.end())
        return NULL;

    return &it->second;;
}

User *Server::findUserByNickname(const std::string &nickname)
{
    std::cout << ">>> LOOKING FOR NICKNAME = [" << nickname << "]" << std::endl;

    for (std::map<int, User>::iterator it = _users.begin();
        it != _users.end();
        ++it)
    {
        std::cout << ">>> USER FD = " << it->first
                  << " NICKNAME = [" << it->second.getNickname() << "]"
                  << std::endl;

        if (it->second.getNickname() == nickname)
            return &it->second;
    }

    return NULL;
}


void Server::handleKick(User &sender, const std::string &arguments)
{
     if (!(sender.getPASS() && sender.getNICK() && sender.getUSER()))
    {
        sendError("451", " :You have not registered", sender);
        return;
    }
    std::cout << ">>> ENTERING handleKick" << std::endl;
    std::vector<std::string> args =
        Parser::getParameters(arguments);


    std::cout << ">>> AFTER PARSER" << std::endl;
    if (args.size() < 2)
    {
        sendKickNeedMoreParams(sender);
        return;
    }

    std::cout << ">>> PARAMETERS OK" << std::endl;

    std::string channelName = args[0];
    std::string targetNickname = args[1];

    std::cout << ">>> CHANNEL NAME = " << channelName << std::endl;

    Channel *channel = findChannel(channelName);

    std::cout << ">>> AFTER findChannel" << std::endl;

    if (channel == NULL)
    {
        std::cout << ">>> CHANNEL IS NULL" << std::endl;
        sendKickNoSuchChannel(sender, channelName);
        return;
    }
    std::cout << ">>> CHANNEL IS NOT NULL" << std::endl;
    // std::cout << ">>> SENDER FD = " << sender.getFD() << std::endl;
    // std::cout << ">>> CHECK SENDER IN CHANNEL" << std::endl;

    std::cout << ">>> BEFORE GET FD" << std::endl;

    int senderFd = sender.getFD();

    std::cout << ">>> AFTER GET FD" << std::endl;
    std::cout << ">>> SENDER FD = " << senderFd << std::endl;
    std::cout << ">>> CHECK SENDER IN CHANNEL" << std::endl;

    if (!channel->hasUser(senderFd))
    {
        std::cout << ">>> SENDER NOT IN CHANNEL" << std::endl;
        sendKickNotOnChannel(sender, channelName);
        return;
    }
    std::cout << ">>> SENDER IS IN CHANNEL" << std::endl;


    std::cout << ">>> CHECK SENDER OPERATOR" << std::endl;

    if(!channel->isOperator(sender.getFD()))
    {
        std::cout << ">>> SENDER IS NOT OPERATOR" << std::endl;
        sendKickNotOperator(sender, channelName);
        return;
    }
    std::cout << ">>> SENDER IS OPERATOR" << std::endl;

    std::cout << ">>> CHECK TARGET USER" << std::endl;
    User *target = findUserByNickname(targetNickname);
    std::cout << ">>> AFTER findUserByNickname" << std::endl;
    if (target == NULL)
    {
        std::cout << ">>> TARGET NOT FOUND" << std::endl;
        sendKickNoSuchNick(sender, targetNickname);
        return;
    }
    std::cout << ">>> TARGET FOUND, FD = " << target->getFD() << std::endl;

    if (!channel->hasUser(target->getFD()))
    {
        std::cout << "TARGET NOT IN CHANNEL" << std::endl;
        sendKickUserNotInChannel(sender, targetNickname, channelName);
        return;
    }

    std::cout << "TARGET IS IN CHANNEL" << std::endl;
    std::string message =
        ":" +  sender.getNickname() +
        " KICK " + channelName +
        " " + targetNickname +
        "\r\n";

    channel->broadcast(*this, message, -1);
    channel->removeUser(target->getFD());

    std::cout << "MESSAGE = ["  << message << "]" << std::endl;
}
