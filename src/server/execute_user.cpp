/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_user.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 15:30:46 by mayoucha          #+#    #+#             */
/*   Updated: 2026/09/15 16:10:31 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"


/*Exemple envoyé par un client :
USER alice 0 * :Alice Dupont*/


void Server::executeUser(User &sender, const std::vector<std::string> &args)
{
	if(args.size() < 4)
		return(this->sendError("461", "USER :Too many parameters", sender));

	if(sender.isRegistered())
		return(this->sendError("462", "USER :User already registered", sender));

	sender.setUsername(args[0]);
	sender.setRealName(args[3]);
	sender.setHasUser(true);

	if(sender.hasPass() && sender.hasNick() && sender.hasUser())
	{
		std::string welcome = ":" + this->getName() + " 001 " + sender.getNickname() + " :Welcome to the Internet Relay Network\r\n";
		this->sendResponse(sender.getFD(), welcome);

		std::string host = ":" + this->getName() + " 002 " + sender.getNickname() + " :Your host is ircserv, running version 1.0\r\n";
		this->sendResponse(sender.getFD(), host);

		std::string created = ":" + this->getName() + " 003 " + sender.getNickname() + " :This server was created today\r\n";
		this->sendResponse(sender.getFD(), created);

		std::string info = ":" + this->getName() + " 004 " + sender.getNickname() + " " + this->getName() + " 1.0 o itkol\r\n";
		this->sendResponse(sender.getFD(), info);

		sender.setRegistered(true);
		return ;
	}
	return ;
}
