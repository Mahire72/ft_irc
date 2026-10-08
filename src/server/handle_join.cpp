/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 18:33:21 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/08 15:17:26 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"
#include <vector>
#include <iostream>


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

    // Si l'utilisateur est déjà dans le canal, rien à faire
    if (channel.isUserInChannel(sender.getFD()))
        return;

    //Mot de passe (+k)
    if (!channel.get_k().empty() && channel.get_k() != key)
    {
        this->sendError("475", channelName + " :Cannot join channel (+k)", sender);
        return;
    }

    //Limite d'utilisateurs (+l)
    if (channel.get_usersLimit() > 0 && channel.getChannelUsersMap().size() >= static_cast<size_t>(channel.get_usersLimit()))
    {
        this->sendError("471", channelName + " :Cannot join channel (+l)", sender);
        return;
    }

    //Sur invitation uniquement (+i)
    if (channel.get_i() && !channel.isInvited(sender.getFD()))
    {
        this->sendError("473", channelName + " :Cannot join channel (+i)", sender);
        return;
    }
    channel.addUser(&sender);
    if (isNew)
        channel.addOperator(sender.getFD());

    // Broadcast JOIN à tous les membres
    std::string message = ":" + sender.getNickname() + " JOIN " + channelName + "\r\n";
    channel.broadcast(*this, message, -1);

    // Topic RPL (332 ou 331)
    if (!channel.getChannelTopic().empty())
        this->sendResponse(sender.getFD(), ":" + this->getName() + " 332 " + sender.getNickname() + " " + channelName + " :" + channel.getChannelTopic() + "\r\n");
    else
        this->sendResponse(sender.getFD(), ":" + this->getName() + " 331 " + sender.getNickname() + " " + channelName + " :No topic is set\r\n");

    // RPL_NAMREPLY (353) & RPL_ENDOFNAMES (366)
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

    // 2. Faire quitter l'utilisateur de chaque canal trouvé
    for (size_t i = 0; i < joinedChannels.size(); ++i)
    {
        std::map<std::string, Channel>::iterator it = _channels.find(joinedChannels[i]);
        if (it != _channels.end())
        {
            Channel &channel = it->second;

            // Message de notification PART (JOIN 0 implique un PART sans message personnalisé)
            std::string partMsg = ":" + sender.getNickname() + " PART " + channel.getChannelName() + "\r\n";

            // Broadcast aux membres du canal (y compris sender selon les clients/specs)
            channel.broadcast(*this, partMsg, -1);

            // Retirer l'utilisateur du canal
            channel.removeUser(sender.getFD());

            // Nettoyage : si le canal devient vide, on le supprime de la map du serveur
            if (channel.getChannelUsersMap().empty())
                _channels.erase(it);
        }
    }
}

void Server::handleJoin(User &sender, const std::string &arguments)
{
    if (!(sender.getRegistered()))
    {
        std::string nick = sender.getNickname().empty() ? "*" : sender.getNickname();
        this->sendError("451", nick + " :You have not registered", sender);
        return;
    }

    std::vector<std::string> args = Parser::getParameters(arguments);
    if (args.empty())
        return (this->sendError("461", "JOIN :Not enough parameters", sender));

    // Cas 1 : JOIN 0 (quitter tous les canaux)
    if (args[0] == "0")
    {
        this->leaveAllChannels(sender); // Fait un PART automatique sur tous ses canaux
        return;
    }

    // Cas 2 : Parsing des canaux et des clés
    std::vector<std::string> channelList = splitByComma(args[0]);
    std::vector<std::string> keyList;

    if (args.size() > 1)
        keyList = splitByComma(args[1]);

    // Boucle sur chaque canal demandé
    for (size_t i = 0; i < channelList.size(); ++i)
    {
        std::string key = (i < keyList.size()) ? keyList[i] : "";
        this->joinSingleChannel(sender, channelList[i], key);
    }
}
