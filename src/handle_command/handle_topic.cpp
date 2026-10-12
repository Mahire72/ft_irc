/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_topic.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 18:46:59 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/11 15:50:28 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"
#include <vector>
#include <iostream>

void Server::handleTopic(User &sender, const std::string &arguments)
{
    if (!(sender.getRegistered()))
    {
        std::string nick = sender.getNickname().empty() ? "*" : sender.getNickname();
        this->sendError("451", nick + " :You have not registered", sender);
        return;
    }

    std::vector<std::string> args =
        Parser::getParameters(arguments);

    if (args.empty())
    {
        sendError("461", "TOPIC :Not enough parameters", sender);
        return;
    }

    std::string channelName = args[0];

    std::map<std::string, Channel>::iterator it =
        _channels.find(channelName);

    if (it == _channels.end())
    {
        sendError("403", channelName + " :No such channel", sender);
        return;
    }

    std::cout << "Channel exists: " << channelName << std::endl;

    Channel &channel = it->second;

    if (!channel.isUserInChannel(sender.getFD()))
    {
        sendError("442", channelName + " :You're not on that channel", sender);
        return;
    }

    if (args.size() == 1)
    {
        if (channel.getChannelTopic().empty())
        {
            sendError("331", channelName + " :No topic is set", sender);
            return;
        }

        std::string response =
            ":" + getName() + " 332 " +
            sender.getNickname() + " " +
            channelName + " :" +
            channel.getChannelTopic() + "\r\n";

        sendResponse(sender.getFD(), response);
        return;
    }

    std::string newTopic = args[1];
    if(channel.get_t())
    {
        if(!sender.isOperator())
            return (this->sendError("482", channel.getChannelName() + " :You're not channel operator", sender));
    }
    channel.setChannelTopic(newTopic);

    std::string message =
              ":" + sender.getNickname() +
              " TOPIC " +
              channelName +
              " :" +
              newTopic +
              "\r\n";

    channel.broadcast(*this, message, -1);
    std::cout << "User is in channel: " << channelName << std::endl;
}
