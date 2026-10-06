/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_invite.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 12:43:38 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/06 16:51:56 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

/*

- There is no requirement that the channel the target user is
  being invited to must exist or be a valid channel.
- However, if the channel exists, only members of the channel
  are allowed to invite other users.

- When the channel has invite-only flag set, only channel operators
  may issue INVITE command.


- seuls celui qui invite et l'inivte recoivent le message d'invitation

- Parameters: <nickname> <channel>


- "INVITE Wiz #Twilight_Zone"  --> commande d'invitation

- ":Angel!wings@irc.org INVITE Wiz #Dust" ---> message d'invitation a WiZ par Angel au #Dusr


*/




//                 INVITE <nickname> <channel>

//
// <nickname>  	arg[0]
// <channel>   	arg[1]




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
