/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:15:28 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/12 17:01:21 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"


Server::Server(int port, const std::string &password)
    : _serverFd(-1), _port(port), _password(password)
{

}

void Server::handlePass(User& user, const std::string& arguments)
{
    if (arguments == _password)
    {
        user.setHasPass(true);
        std::cout << "PASS correct! hasPass = "
                  << user.hasPass() << std::endl;
    }
    else
        sendResponse(user.getFD(), ":ircserv 464 * :Password incorrect\r\n");
}
// 464 is the IRC numeric for ERR_PASSWDMISMATCH


std::string Server::getName() const
{
    return(_name);
}

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
