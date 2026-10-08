/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_user.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 15:03:38 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/08 15:15:03 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"

#include <vector>
#include <iostream>

void Server::handleUser(User &user, const std::string &arguments)
{
    std::vector<std::string> parameters =
    Parser::getParameters(arguments);

    if (parameters.size() < 4) // parameter user need 4 parameters
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

    std::cout << "USER accepted: username=["
              << user.getUsername()
              << "] realname=["
              << user.getRealName()
              << "]"
              << std::endl;
}
