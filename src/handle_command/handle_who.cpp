/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_who.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 19:46:53 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/14 12:26:11 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

void Server::handleWho(User &sender, const std::vector<std::string> &arguments)
{
    std::string target = arguments.empty() ? "*" : arguments[0];
    std::string response = ":" + this->getName() + " 315 " + sender.getNickname() + " " + target + " :End of /WHO list.\r\n";

    sendResponse(sender.getFD(), response);
}
