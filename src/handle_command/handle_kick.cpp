/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_kick.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 17:49:35 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 12:44:52 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"


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

    return &it->second;
}

User *Server::findUserByNickname(const std::string &nickname)
{
    for (std::map<int, User>::iterator it = _users.begin();
        it != _users.end();
        ++it)
    {
        if (it->second.getNickname() == nickname)
            return &it->second;
    }
    return NULL;
}


void Server::handleKick(User &sender, const std::string &arguments)
{
    if (!(sender.getRegistered()))
    {
        std::string nick = sender.getNickname().empty() ? "*" : sender.getNickname();
        this->sendError("451", nick + " :You have not registered", sender);
        return;
    }

    std::vector<std::string> args =
        Parser::getParameters(arguments);

    if (args.size() < 2)
    {
        sendKickNeedMoreParams(sender);
        return;
    }

    std::string channelName = args[0];
    std::string targetNickname = args[1];
    std::string comment;

    for (size_t i = 2; i < args.size(); ++i)
    {
        if (i > 2)
            comment = comment + " ";
        comment += args[i];

    }

    Channel *channel = findChannel(channelName);

    if (channel == NULL)
    {
        sendKickNoSuchChannel(sender, channelName);
        return;
    }

    int senderFd = sender.getFD();


    if (!channel->isUserInChannel(senderFd))
    {
        sendKickNotOnChannel(sender, channelName);
        return;
    }

    if(!channel->isOperator(sender.getFD()))
    {
        sendKickNotOperator(sender, channelName);
        return;
    }

    User *target = findUserByNickname(targetNickname);
    if (target == NULL)
    {
        sendKickNoSuchNick(sender, targetNickname);
        return;
    }

    if (!channel->isUserInChannel(target->getFD()))
    {
        sendKickUserNotInChannel(sender, targetNickname, channelName);
        return;
    }

    std::string message =
        ":" +  sender.getNickname() +
        " KICK " + channelName +
        " " + targetNickname;

    if (!comment.empty())
        message = message + " :" + comment;

    message = message + "\r\n";

    channel->broadcast(*this, message, -1);
    channel->removeUser(target->getFD());
}
