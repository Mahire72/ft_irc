/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:15:28 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/14 14:16:47 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

#include <unistd.h>

Server::Server(int port, const std::string &password)
    : _serverFd(-1), _port(port), _password(password)
{
    
}

void Server::handlePass(User& user, const std::string& arguments)
{
    if (arguments.empty())
    {
        sendResponse(user.getFD(), ":ircserv 461 * PASS :Not enough parameters\r\n");
        return;
    }
    
    if (user.isRegistered())
    {
        sendResponse(user.getFD(),
            ":IRCSERV 462 * :You may not register\r\n");
        return;
    }

    if (arguments == _password)
    {
        user.setHasPass(true);
        return;
    }
    
    //otherwise
    sendResponse(user.getFD(), ":ircserv 464 * :Password incorrect\r\n");
    // if (arguments == _password)
    // {
    //     user.setHasPass(true);
    //     std::cout << "PASS correct! hasPass = "
    //               << user.hasPass() << std::endl;
    // }
    // else
    //     sendResponse(user.getFD(), ":ircserv 464 * :Password incorrect\r\n");
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



Server::~Server()
{
    for (size_t i = 0; i < _pollFds.size(); ++i)
        close(_pollFds[i].fd);
}

void Server::start()
{
    setupSocket();
    setupAddress();
    startListening();

    addPollFd(_serverFd);

    run();
}