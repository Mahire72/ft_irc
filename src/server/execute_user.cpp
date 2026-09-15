/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_user.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 15:30:46 by mayoucha          #+#    #+#             */
/*   Updated: 2026/09/15 16:49:46 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"


/*Exemple envoyé par un client :
USER alice 0 * :Alice Dupont*/


void Server::executeUser(User &sender, const std::vector<std::string> &args)
{
	if(args.size() < 4)
		return(this->sendError("461", "USER :Not enough parameters", sender));

	if(sender.isRegistered())
		return(this->sendError("462", "USER :User already registered", sender));

	sender.setUsername(args[0]);
	sender.setRealName(args[3]);
	sender.setHasUser(true);

	this->checkRegistration(sender);
	return ;
}
