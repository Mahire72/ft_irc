/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_user.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 15:03:38 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 12:42:07 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

void Server::handleUser(User &user, const std::string &arguments)
{
    std::vector<std::string> parameters =
    Parser::getParameters(arguments);

    if (parameters.size() < 4)
    {
        sendResponse(user.getFD(),
            ":ircserv 461 * USER :Not enough parameters\r\n");
        return;
    }
    if (user.hasUser())
    {
        sendResponse(user.getFD(),
            ":ircserv 462 * :You may not reregister\r\n");
        return;
    }
    user.setUsername(parameters[0]);
    user.setRealName(parameters[3]);
    user.setHasUser(true);
    user.setUSER(true);
    checkRegistration(user);
 }
