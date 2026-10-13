/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ping.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 13:39:00 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/13 19:49:11 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../../include/Server.hpp"
 #include "../../include/Parser.hpp"


void Server::handlePing(User &sender, const std::string &arguments)
{
    std::vector<std::string> args =
        Parser::getParameters(arguments);

    if (args.empty())
        return;

    std::string token = args[0];

    std::string response =
        ":" + getName() + " PONG "
        + getName() + " :" + token;

    sendResponse(sender.getFD(), response);
}
