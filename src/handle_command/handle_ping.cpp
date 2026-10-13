/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ping.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 13:39:00 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/13 15:52:40 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"


void Server::handlePing(User &sender, const std::string &arguments)
{
    std::vector<std::string> args = Parser::getParameters(arguments);

    if (args.size() < 2)
        return;

    std::string ping_id = args[1];

    sender.setPingId(ping_id);

    std::string response = ":" + this->getName() + " PONG " + this->getName() +
    " :" + sender.getPingId() + "\r\n";

    this->sendResponse(sender.getFD(), response);
}
