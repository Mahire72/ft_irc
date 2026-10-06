/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_invite.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 12:43:38 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/06 18:33:05 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

/*
irc:      :Alice!alice@ INVITE Charlie :#42 (manque IP)
attendu : :Alice!alice@127.0.0.1 INVITE Charlie :#salon
*/


void Server::handleInvite(User &sender, const std::string &arguments)
{
    std::vector<std::string> args = Parser::getParameters(arguments);

    if (args.size() < 2)
        return (this->sendError("461", "INVITE :Not enough parameters", sender));

    if(!this->isUserInServer(args[0]))
        return (this->sendError("401", args[0] + " :No such nickname in server", sender));

    int targetFd = this->getUserFd(args[0]);

    std::map<std::string, Channel>::iterator it = _channels.find(args[1]);
    if (it == _channels.end())
        return (this->sendError("403", args[1] + " :No such channel", sender));
    Channel &channel = it->second;

    if (!channel.isUserInChannel(sender.getFD()))
        return (this->sendError("442", args[1] + " :You're not on that channel ", sender));

    if (channel.get_i())
    {
        if (!channel.isOperator(sender.getFD()))
          return (this->sendError("482", args[1] + " :You're not channel operator", sender));
    }

    if(channel.isUserInChannel(targetFd))
      return (this->sendError("443", args[0] + " " + args[1] + " :Is already in channel ", sender));

    else
    {
      channel.addInvite(targetFd);

      this->sendResponse(sender.getFD(), ":" + this->getName() + " 341 " +
      sender.getNickname() + " " + args[1] + " " + args[0]);

      this->sendResponse(targetFd, ":" + sender.getNickname() + "!" +
      sender.getUsername() + "@" + sender.getIP() + " INVITE " + args[0] + " :" + args[1]);
    }
}
