/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pong.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 17:23:09 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/13 18:40:26 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/User.hpp"
#include "../../include/Parser.hpp"

void Server::handlePong(User &sender, const std::string &arguments)
{
    std::vector<std::string> args =
        Parser::getParameters(arguments);

    if (args.empty())
        return;

    std::string token = args[0];

    if (token == sender.getPingId())
    {
        sender.setPingId("");
        
        std::cout << "PONG received from fd "
                  << sender.getFD() << std::endl;
    }
}
