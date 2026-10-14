/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_topic.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 18:46:59 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 13:39:26 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

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
        if(!channel.isOperator(sender.getFD()))
        {
            std::cout << "operateur est : " + sender.getNickname()  << std::endl;
            return (this->sendError("482", channel.getChannelName() + " :You're not channel operator", sender));
        }
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
}
