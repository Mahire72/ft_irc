/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:40:08 by mayoucha          #+#    #+#             */
/*   Updated: 2026/09/12 19:25:10 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

//Here is important to understand how the message is filled in args[...]
// Option 1 : args[1] contains the entire message, for example "args[1] = Hello world!"
// Option 2 : the message was split into several words and args[1] = "Hello", args[2] = "world!" etc.

//Here I managed it following the option 1, I can adapt it later if needed

void Server::executePrivmsg(User &sender, const std::vector<std::string> &args)
{
	if(args.empty())
	{
		std::string err = ":" + this->getName() + " 411 " + sender.getNickname() + " PRIVMSG :No recipient\r\n";
		this->sendResponse(sender.getFD(), err);
		return ;
	}
	else if(args.size() == 1)
	{
		std::string err = ":" + this->getName() + " 412 " + sender.getNickname() + " PRIVMSG :No text to send\r\n";
		this->sendResponse(sender.getFD(), err);
		return ;
	}

	std::string target = args[0];
	std::string message = args[1];
	std::string formattedMsg = ":" + sender.getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";

	if(target[0] == '#' || target[0] == '&')
	{
		std::map<std::string, Channel>::iterator it = _channels.find(args[0]); //cherche si le canal existe deja
		if(it == _channels.end())
		{
			std::string err = ":" + this->getName() + " 401 " + sender.getNickname() + " " + target + " :No such nick/channel\r\n";
			this->sendResponse(sender.getFD(), err);
			return ;
		}
		else
		{
			it->second.broadcast(*this, formattedMsg, sender.getFD());
		}
	}
	else
	{
		for(std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it)
		{
			if(it->second.getNickname() == target)
			{
				this->sendResponse(it->second.getFD(), formattedMsg);
				return ;
			}
		}
		std::string err = ":" + this->getName() + " 401 " + sender.getNickname() + " " + target + " :No such nick/channel\r\n";
		this->sendResponse(sender.getFD(), err);
		return ;
	}
}
