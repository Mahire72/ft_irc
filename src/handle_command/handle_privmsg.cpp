/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_privmsg.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:40:08 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/10 16:19:46 by mayoucha         ###   ########.fr       */
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

    // Extraction brute :args[0] = destinataire, args[1] = message complet
    std::vector<std::string> args = Parser::getParameters(arguments);

    if (args.empty())
        return this->sendError("411", " :No recipient given", sender);

    if (args.size() < 2 || args[1].empty())
        return this->sendError("412", " :No text to send", sender);

    std::string target = args[0];
    std::string message = args[1]; // Doit contenir le texte complet apres le ':'

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


/*
            PRIVMSG
              │
              ↓
       Recipient given?
          /          \
        NO            YES
        ↓              ↓
 ERR_NORECIPIENT   Text to send given?
                     /       \
                   NO         YES
                   ↓           ↓
             ERR_NOTEXT    Is target a Channel (# / &) or User?
                             /            \
                       CHANNEL            USER
                          ↓                ↓
                 Is user in channel?   Does user exist?
                    /         \           /       \
                  NO          YES        NO       YES
                  ↓            ↓         ↓         ↓
            ERR_CANNOT     Broadcast   ERR_NOSUCH  Forward direct
               SEND        to channel     NICK     message to user
               TOCHAN      (excl. sender)

*/

