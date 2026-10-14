/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_privmsg.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:40:08 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/14 12:45:30 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"


void Server::handlePrivmsg(User &sender, const std::string &arguments)
{
    if (!sender.getRegistered())
    {
        std::string nick = sender.getNickname().empty() ? "*" : sender.getNickname();
        this->sendError("451", nick + " :You have not registered", sender);
        return;
    }

    std::vector<std::string> args = Parser::getParameters(arguments);

    if (args.empty())
        return this->sendError("411", " :No recipient given", sender);

    if (args.size() < 2 || args[1].empty())
        return this->sendError("412", " :No text to send", sender);

    std::string target = args[0];
    std::string message = args[1]; 

    std::string host = sender.getIP().empty() ? "127.0.0.1" : sender.getIP();
    std::string fullMsg = ":" + sender.getNickname() + "!" + sender.getUsername()
                        + "@" + host + " PRIVMSG " + target + " :" + message + "\r\n";

    if (target[0] == '#' || target[0] == '&')
    {
        std::map<std::string, Channel>::iterator it = _channels.find(target);
        if (it == _channels.end())
            return this->sendError("401", target + " :No such nick/channel", sender);

        if (!it->second.isUserInChannel(sender.getFD()))
            return this->sendError("404", target + " :Cannot send to channel", sender);

        it->second.broadcast(*this, fullMsg, sender.getFD());
    }
    else
    {
        for (std::map<int, User>::iterator it2 = _users.begin(); it2 != _users.end(); ++it2)
        {
            if (it2->second.getNickname() == target)
            {
                this->sendResponse(it2->second.getFD(), fullMsg);
                return;
            }
        }
        this->sendError("401", target + " :No such nick/channel", sender);
    }
}

