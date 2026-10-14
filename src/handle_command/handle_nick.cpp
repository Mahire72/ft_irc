/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_nick.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:15:11 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 12:45:12 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

bool isLetter(char c)
{
    return ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'));
}

bool isSpecial(char c)
{
    return ((c >= '[' && c <= '`') ||
            (c >= '{' && c <= '}'));
}

bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

bool Server::isValidNickname(const std::string& nickname)
{
    if (nickname.empty() || nickname.size() > 9)
        return false;

    if (!isLetter(nickname[0]) && !isSpecial(nickname[0]))
        return false;

    for (size_t i = 1; i < nickname.size(); ++i)
    {
        char c = nickname[i];
        if (!isLetter(c) && !isDigit(c) && !isSpecial(c) && c != '-')
            return false;
    }

    return true;
}

bool Server::isNicknameTaken(const std::string& nickname)
{
    for (std::map<int, User>::iterator it = _users.begin();
        it != _users.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
        {
            return true;
        }
    }
    return false;
}

void Server::handleNick(User& user, const std::string& arguments)
{
    if (arguments.empty())
    {
        sendResponse(user.getFD(),
            ":IRCSERV 431 * :No nickname given\r\n");
        return;
    }

    if (!isValidNickname(arguments))
    {
        sendResponse(user.getFD(),
            ":ircserv 432 " + arguments + " :Erroneous nickname\r\n");
        return;
    }

    if (isNicknameTaken(arguments))
    {
        sendResponse(user.getFD(),
            ":ircserv 433 " + arguments + " :Nickname is already in use\r\n");
        return;
    }
    user.setNickname(arguments);
    user.setNICK(true);
    checkRegistration(user);
}
