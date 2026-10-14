/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 18:33:21 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 12:27:10 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"


void Server::joinSingleChannel(User &sender,
const std::string &channelName, const std::string &key)
{
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
    {
        this->sendError("403", channelName + " :No such channel", sender);
        return ;
    }

    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    bool isNew = (it == _channels.end());
    if (isNew)
    {
        std::pair<std::map<std::string, Channel>::iterator, bool> ret;
        ret = _channels.insert(std::make_pair(channelName, Channel(channelName)));
        it = ret.first;
    }

    Channel &channel = it->second;

    if (channel.isUserInChannel(sender.getFD()))
        return;

    if (!channel.get_k().empty() && channel.get_k() != key)
    {
        this->sendError("475", channelName + " :Cannot join channel (+k)", sender);
        return;
    }

    if (channel.get_usersLimit() > 0 && channel.getChannelUsersMap().size() >= static_cast<size_t>(channel.get_usersLimit()))
    {
        this->sendError("471", channelName + " :Cannot join channel (+l)", sender);
        return;
    }

    if (channel.get_i() && !channel.isInvited(sender.getFD()))
    {
        this->sendError("473", channelName + " :Cannot join channel (+i)", sender);
        return;
    }
    channel.addUser(&sender);
    if (isNew)
        channel.addOperator(sender.getFD());

    std::string message = ":" + sender.getNickname() + "!" + sender.getUsername() + "@" + sender.getIP() + " JOIN " + channelName + "\r\n";
    channel.broadcast(*this, message, -1);

    if (!channel.getChannelTopic().empty())
        this->sendResponse(sender.getFD(), ":" + this->getName() + " 332 " + sender.getNickname() + " " + channelName + " :" + channel.getChannelTopic() + "\r\n");
    else
        this->sendResponse(sender.getFD(), ":" + this->getName() + " 331 " + sender.getNickname() + " " + channelName + " :No topic is set\r\n");

    std::string namesList = channel.getChannelUsers();
    this->sendResponse(sender.getFD(), ":" + this->getName() + " 353 " + sender.getNickname() + " = " + channelName + " :" + namesList + "\r\n");
    this->sendResponse(sender.getFD(), ":" + this->getName() + " 366 " + sender.getNickname() + " " + channelName + " :End of /NAMES list\r\n");
}


std::vector<std::string> Server::splitByComma(const std::string &str)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, ','))
    {
        if (!item.empty())
            tokens.push_back(item);
    }
    return (tokens);
}
void Server::leaveAllChannels(User &sender)
{
    std::vector<std::string> joinedChannels;

    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        Channel &channel = it->second;
        if (channel.isUserInChannel(sender.getFD()))
            joinedChannels.push_back(channel.getChannelName());
    }

    for (size_t i = 0; i < joinedChannels.size(); ++i)
    {
        std::map<std::string, Channel>::iterator it = _channels.find(joinedChannels[i]);
        if (it != _channels.end())
        {
            Channel &channel = it->second;

            std::string partMsg = ":" + sender.getNickname() + " PART " + channel.getChannelName() + "\r\n";

            channel.broadcast(*this, partMsg, -1);

            channel.removeUser(sender.getFD());

            if (channel.getChannelUsersMap().empty())
                _channels.erase(it);
        }
    }
}

void Server::handleJoin(User &sender, const std::string &arguments)
{
    if (!(sender.getRegistered()))
    {
        this->sendError("451", ":You have not registered", sender);
        return;
    }

    std::vector<std::string> args = Parser::getParameters(arguments);
    if (args.empty())
        return (this->sendError("461", "JOIN :Not enough parameters", sender));

    if (args[0] == "0")
    {
        this->leaveAllChannels(sender); 
        return;
    }

    std::vector<std::string> channelList = splitByComma(args[0]);
    std::vector<std::string> keyList;

    if (args.size() > 1)
        keyList = splitByComma(args[1]);

    for (size_t i = 0; i < channelList.size(); ++i)
    {
        std::string key = (i < keyList.size()) ? keyList[i] : "";
        this->joinSingleChannel(sender, channelList[i], key);
    }
}