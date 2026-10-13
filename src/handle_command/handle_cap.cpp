/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cap.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 15:44:26 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/13 15:46:17 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

void Server::handleCap(User &sender, const std::string &arguments)
{
    // La commande CAP est envoyée avant l'enregistrement,
    // on ne vérifie donc pas getRegistered() ici.

    std::vector<std::string> args = Parser::getParameters(arguments);
    std::string response;

    if (!args.empty() && args[0] == "LS")
        response = ":" + this->getName() + " CAP * LS :\r\n";

    else if (!args.empty() && args[0] == "REQ")
    {
        std::string req = (args.size() > 1) ? args[1] : "";
        response = ":" + this->getName() + " CAP * NAK :" + req + "\r\n";
    }

    else if (!args.empty() && args[0] == "END")
        return;

    else
        response = ":" + this->getName() + " CAP * LS :\r\n";

    this->sendResponse(sender.getFD(), response);
}
