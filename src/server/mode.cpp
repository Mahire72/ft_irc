/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:29:44 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/04 16:47:50 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

/*
MODE - Change the channel’s mode:
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privilege
· l: Set/remove the user limit to channel
*/

void Server::displayChannelModes(User &sender, Channel &channel)
{
    std::string activeModes = "+";

    if (channel.get_i()) activeModes += "i";
    if (channel.get_t()) activeModes += "t";
    if (!channel.get_k().empty()) activeModes += "k";
    if (channel.get_usersLimit() > 0) activeModes += "l";

    std::string rpl324 = ":" + this->getName() + " 324 " + sender.getNickname() + " " + channel.getChannelName() + " " + activeModes + "\r\n";
    this->sendResponse(sender.getFD(), rpl324);
}

void Server::applySimpleModes(User &sender, Channel &channel, const std::string &flags)
{
    bool adding = true;

    for (size_t i = 0; i < flags.size(); ++i)
    {
        char c = flags[i];

        if (c == '+')
            adding = true;
        else if (c == '-')
            adding = false;
        else if (c == 'i')
            channel.set_i(adding);
        else if (c == 't')
            channel.set_t(adding);
        else if (c == 'k' || c == 'o' || c == 'l')
		{
            // TODOD
		}
        else
            this->sendError("472", std::string(1, c) + " :is unknown mode char to me", sender);
    }
}

void Server::handleMode(User &sender, const std::string &arguments)
{
    std::vector<std::string> args = Parser::getParameters(arguments);

    if (args.empty())
        return (this->sendError("461", "MODE :Not enough parameters", sender));

    std::map<std::string, Channel>::iterator it = _channels.find(args[0]);
    if (it == _channels.end())
        return (this->sendError("403", args[0] + " :No such channel", sender));

    Channel &channel = it->second;

    //Cas 1: Consultation des modes (MODE #channel)
    if (args.size() == 1)
    {
        this->displayChannelModes(sender, channel);
        return;
    }

    //Cas 2: Verification des droits de ChanOperator
    if (!channel.isOperator(sender.getFD()))
        return (this->sendError("482", args[0] + " :You're not channel operator", sender));

    //Cas 3: Application des modifications et broadcast
    this->applySimpleModes(sender, channel, args[1]);

    std::string message = ":" + sender.getNickname() + " MODE " + args[0] + " " + args[1] + "\r\n";
    channel.broadcast(*this, message, -1);
}

