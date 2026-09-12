/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 14:34:51 by mayoucha          #+#    #+#             */
/*   Updated: 2026/09/12 16:50:23 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

//// RAPPEL : commande ressemble a "JOIN #canal1"

////// TO CODE getName dans Serveur !!!!!!!!!!!!!!!!!
////// TO CODE getNameList dans Channel !!!!!!!!!!!!!!!!!


void Server::executeJoin(User &sender, const std::vector<std::string> &args)
{
	if(args.empty())
	{
		std::string err = ":" + this->getName() + " 461 " + sender.getNickname() + " JOIN :Not enough parameters\r\n";
		this->sendResponse(sender.getFD(), err);
		return ;

	}

	if(args[0][0] != '#' && args[0][0] != '&')
		return ;

	std::map<std::string, Channel>::iterator it = _channels.find(args[0]); //cherche si le canal existe deja

	if(it == _channels.end())
	{
		_channels[args[0]] = Channel(args[0]);
	}
	Channel &channel = _channels[args[0]];
	channel.addUser(&sender);

	std::string message = ":" + sender.getNickname() + " JOIN " + args[0] + "\r\n";
	channel.broadcast(*this, message, -1);

	std::string namesList = channel.getNamesList();
	// 1. RPL_NAMREPLY (353)
	std::string rpl353 = ":" + this->getName() + " 353 " + sender.getNickname() + " = " + args[0] + " :" + namesList + "\r\n";
	this->sendResponse(sender.getFD(), rpl353);

	// 2. RPL_ENDOFNAMES (366)
	std::string rpl366 = ":" + this->getName() + " 366 " + sender.getNickname() + " " + args[0] + " :End of /NAMES list\r\n";
	this->sendResponse(sender.getFD(), rpl366);
}



	/*if(it != _channels.end()) // si existe, on le recupere
	{
		Channel &channel = it->second;
		channel.addUser(&sender);
		std::string message = ":" + sender.getNickname() + " JOIN " + args[0] + "\r\n";
		channel.broadcast(*this, message, -1);

	}
	else
	{
		_channels[args[0]] = Channel(args[0]);
		_channels[args[0]].addUser(&sender);

	}*/
