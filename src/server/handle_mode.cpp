/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_mode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:29:44 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/08 15:14:26 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

/*
MODE - Change the channel’s mode:
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel operators
· k: Set/remove the channel key (password)
· l: Set/remove the user limit to channel
· o: Give/take channel operator privilege
*/


std::string Server::applyModeKey(User &sender, Channel &channel, const std::vector<std::string> &args, size_t &paramIndex, bool adding)
{
    if (adding)
    {
        if (paramIndex < args.size())
        {
            std::string key = args[paramIndex++];
            channel.set_k(key);
            return "+k " + key;
        }
        this->sendError("461", "MODE :Not enough parameters", sender);
        return "";
    }
    channel.set_k("");
    return "-k";
}

std::string Server::applyModeLimit(User &sender, Channel &channel, const std::vector<std::string> &args, size_t &paramIndex, bool adding)
{
    if (adding)
    {
        if (paramIndex < args.size())
        {
            int userMax = std::atoi(args[paramIndex++].c_str());
            if (userMax > 0)
            {
                channel.set_usersLimit(userMax);
                return "+l " + args[paramIndex - 1];
            }
            return ""; // Limite invalide (<= 0), rien n'est appliqué
        }
        this->sendError("461", "MODE :Not enough parameters", sender);
        return "";
    }
    channel.set_usersLimit(0);
    return "-l";
}

std::string Server::applyModeOperator(User &sender, Channel &channel, const std::vector<std::string> &args, size_t &paramIndex, bool adding)
{
    if (paramIndex >= args.size())
    {
        this->sendError("461", "MODE :Not enough parameters", sender);
        return "";
    }

    std::string targetNick = args[paramIndex++];
    int targetFd = -1;
    const std::map<int, User*> &channelUsers = channel.getChannelUsersMap();

    for (std::map<int, User*>::const_iterator itUser = channelUsers.begin(); itUser != channelUsers.end(); ++itUser)
    {
        if (itUser->second && itUser->second->getNickname() == targetNick)
        {
            targetFd = itUser->first;
            break;
        }
    }

    if (targetFd == -1)
    {
        this->sendError("441", targetNick + " " + channel.getChannelName() + " :They isn't on that channel", sender);
        return "";
    }

    if (adding)
    {
        channel.addOperator(targetFd);
        return "+o " + targetNick;
    }
    channel.removeOperator(targetFd);
    return "-o " + targetNick;
}

std::string Server::applyModes(User &sender, Channel &channel, const std::vector<std::string> &args)
{
    bool adding = true;
    size_t paramIndex = 2;
    std::string changes = "";

    for (size_t i = 0; i < args[1].size(); ++i)
    {
        char c = args[1][i];
        std::string res = "";

        if (c == '+')
            adding = true;
        else if (c == '-')
            adding = false;
        else if (c == 'i')
        {
            channel.set_i(adding);
            res = (adding ? "+i" : "-i");
        }
        else if (c == 't')
        {
            channel.set_t(adding);
            res = (adding ? "+t" : "-t");
        }
        else if (c == 'k')
            res = applyModeKey(sender, channel, args, paramIndex, adding);
        else if (c == 'l')
            res = applyModeLimit(sender, channel, args, paramIndex, adding);
        else if (c == 'o')
            res = applyModeOperator(sender, channel, args, paramIndex, adding);
        else
            this->sendError("472", std::string(1, c) + " :is unknown mode char to me", sender);

        if (!res.empty())
        {
            if (!changes.empty())
                changes += " ";
            changes += res;
        }
    }
    return changes;
}


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

void Server::handleMode(User &sender, const std::string &arguments)
{
    if (!(sender.getPASS() && sender.getNICK() && sender.getUSER()))
    {
        std::string nick = sender.getNickname().empty() ? "*" : sender.getNickname();
        this->sendError("451", nick + " :You have not registered", sender);
        return;
    }

    std::vector<std::string> args = Parser::getParameters(arguments);

    if (args.empty())
        return (this->sendError("461", "MODE :Not enough parameters", sender));

    std::map<std::string, Channel>::iterator it = _channels.find(args[0]);
    if (it == _channels.end())
        return (this->sendError("403", args[0] + " :No such channel", sender));

    Channel &channel = it->second;

    if (args.size() == 1)
    {
        this->displayChannelModes(sender, channel);
        return;
    }

    if (!channel.isOperator(sender.getFD()))
        return (this->sendError("482", args[0] + " :You're not channel operator", sender));

    std::string appliedModes = this->applyModes(sender, channel, args);
    if (!appliedModes.empty())
    {
        std::string message = ":" + sender.getNickname() + " MODE " + channel.getChannelName() + " " + appliedModes + "\r\n";
        channel.broadcast(*this, message, -1);
    }
}



/*void Server::applyModes(User &sender, Channel &channel, const std::vector<std::string> &args)
{
    bool adding = true;
	size_t paramIndex = 2;

    for (size_t i = 0; i < args[1].size(); ++i)
    {
        char c = args[1][i];

        if (c == '+')
            adding = true;
        else if (c == '-')
            adding = false;
        else if (c == 'i')
            channel.set_i(adding);
        else if (c == 't')
            channel.set_t(adding);
        else if (c == 'k')
		{
            if(adding)
            {
                if(paramIndex < args.size())
                {
                    channel.set_k(args[paramIndex]);
                    paramIndex++;
                }
                else
                    return (this->sendError("461", "MODE :Not enough parameters", sender));
            }
            else
                channel.set_k("");
        }
        else if (c == 'l')
        {
            if(adding)
            {
                if(paramIndex < args.size())
                {
                    int userMax = std::atoi(args[paramIndex].c_str());
                    paramIndex++;
                    if(userMax > 0)
                        channel.set_usersLimit(userMax);
                }
                else
                    return (this->sendError("461", "MODE :Not enough parameters", sender));
            }
            else
                channel.set_usersLimit(0);
        }

        else if (c == 'o')
        {
            if (paramIndex >= args.size())
                return (this->sendError("461", "MODE :Not enough parameters", sender));

            std::string targetNick = args[paramIndex];
            paramIndex++;

            int targetFd = -1;
            const std::map<int, User*> &channelUsers = channel.getChannelUsersMap();

            for (std::map<int, User*>::const_iterator itUser = channelUsers.begin(); itUser != channelUsers.end(); ++itUser)
            {
                if (itUser->second && itUser->second->getNickname() == targetNick)
                {
                    targetFd = itUser->first; // ou itUser->second->getFD()
                    break;
                }
            }
            if (targetFd == -1)
                return (this->sendError("441", targetNick + " " + channel.getChannelName() + " :They isn't on that channel", sender));
            if (adding)
                channel.addOperator(targetFd);
            else
                channel.removeOperator(targetFd);
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
    this->applyModes(sender, channel, args);

    std::string message = ":" + sender.getNickname() + " MODE " + args[0] + " " + args[1] + "\r\n";
    channel.broadcast(*this, message, -1);
}

*/
