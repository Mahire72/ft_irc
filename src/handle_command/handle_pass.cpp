/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/11 00:14:00 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/11 15:49:04 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include <ctime>

void Server::handlePass(User& user, const std::string& arguments)
{
    if (arguments.empty())
    {
        sendResponse(user.getFD(), ":ircserv 461 * PASS :Not enough parameters\r\n");
        return;
    }

    if (user.getRegistered() || user.hasPass())
    {
        sendResponse(user.getFD(),
            ": IRCSERV 462 * :Unauthorized command (already registered)\r\n");
        return;
    }

    if (arguments == _password)
    {
        user.setHasPass(true);
        user.setPASS(true);
        checkRegistration(user); 
        return;
    }
    sendResponse(user.getFD(), ":ircserv 464 * :Password incorrect\r\n");
}
/*

             PASS
              │
              ↓
       Does it have argument?
          /          \
        NO            YES
        ↓              ↓
 ERR_NEEDMORE     Already registered?
                     /       \
                   YES         NO
                   ↓            ↓
          ERR_ALREADY     Compare password
          REGISTERED          /      \
                             OK       WRONG
                             ↓          ↓
                      hasPass=true   error
*/

std::string Server::getCurrentTime()
{
    std::time_t now = std::time(NULL);
    std::tm *timeInfo = std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%B %d %Y %H:%M:%S", timeInfo);

    return std::string(buffer);
}

/*
%B --full month name
%d --day of the month
%Y --year
*/

void Server::checkRegistration(User &user)
{
    if (user.getRegistered())
    {
        sendResponse(user.getFD(),
            "462 :Unauthorized command (already registered)\r\n");
        return  ;
    }
    if (user.getPASS() && user.getNICK() && user.getUSER())
    {
        user.setRegistered(true);

        std::string nick = user.getNickname();
        std::string host = this->getName();

        sendResponse(user.getFD(), ":" + host + " 001 " + nick + " :Welcome to the IRC Network " + nick + "\r\n");
        sendResponse(user.getFD(), ":" + host + " 002 " + nick + " :Your host is " + host + ", running version 1.0\r\n");
        sendResponse(user.getFD(), ":" + host + " 003 " + nick + " :This server is created " +getCurrentTime() + "\r\n");
        sendResponse(user.getFD(), ":" + host + " 004 " + nick + " " + host + " 1.0 itkol\r\n");
        }
}
