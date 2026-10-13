/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ping.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 13:39:00 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/13 18:24:19 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../../include/Server.hpp"
 #include "../../include/Parser.hpp"


// void Server::handlePing(User &sender, const std::string &arguments)
// {
//     std::vector<std::string> args = Parser::getParameters(arguments);

//     if (args.size() < 2)
//         return;

//     std::string ping_id = args[1];

//     sender.setPingId(ping_id);

//     std::string response = ":" + this->getName() + " PONG " + this->getName() +
//     " :" + sender.getPingId() + "\r\n";

//     this->sendResponse(sender.getFD(), response);
// }


void Server::handlePing(User &sender, const std::string &arguments)
{
    std::vector<std::string> args =
        Parser::getParameters(arguments);

    if (args.empty())
    {
        // sendResponse(sender.getFD(),
        //     ":" + getName() + " 409 "
        //     + sender.getNickname()
        //     + " :No origin specified");
        return;
    }

    std::string token = args[0];

    std::string response =
        ":" + getName() + " PONG "
        + getName() + " :" + token;

    sendResponse(sender.getFD(), response);
}
