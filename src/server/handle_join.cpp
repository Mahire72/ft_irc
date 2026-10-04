/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 14:34:51 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/04 12:31:44 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

//// RAPPEL : commande ressemble a "JOIN #canal1"

void Server::handleJoin(User &sender, const std::vector<std::string> &args)
{
	if(args.empty())
		return(this->sendError("461", "JOIN :Not enough parameters", sender));

	if(args[0][0] != '#' && args[0][0] != '&')
		return ;

	std::map<std::string, Channel>::iterator it = _channels.find(args[0]); //cherche si le canal existe deja
	if(it == _channels.end())
	{
		//_channels[args[0]] = Channel(args[0]);
		std::pair<std::map<std::string, Channel>::iterator, bool> ret;
    	ret = _channels.insert(std::make_pair(args[0], Channel(args[0])));
    	it = ret.first;
	}

	Channel &channel = it->second;

	channel.addUser(&sender);

	std::string message = ":" + sender.getNickname() + " JOIN " + args[0] + "\r\n";
	channel.broadcast(*this, message, -1);


	if (!channel.getChannelTopic().empty())
	{
    	std::string rpl332 = ":" + this->getName() + " 332 " + sender.getNickname() + " " + args[0] + " :" + channel.getTopic() + "\r\n";
    	this->sendResponse(sender.getFD(), rpl332);
	}
	
	else
	{
		std::string rpl331 = ":" + this->getName() + " 331 " + sender.getNickname() + " " + args[0] + " :No topic is set\r\n";
		this->sendResponse(sender.getFD(), rpl331);
	}

	// 1. RPL_NAMREPLY (353)
	std::string namesList = channel.getChannelUsers();
	std::string rpl353 = ":" + this->getName() + " 353 " + sender.getNickname() + " = " + args[0] + " :" + namesList + "\r\n";
	this->sendResponse(sender.getFD(), rpl353);

	// 2. RPL_ENDOFNAMES (366)
	std::string rpl366 = ":" + this->getName() + " 366 " + sender.getNickname() + " " + args[0] + " :End of /NAMES list\r\n";
	this->sendResponse(sender.getFD(), rpl366);
}
