/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:15:28 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/15 12:16:10 by mayoucha         ###   ########.fr       */
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


void Server::sendError(const std::string &err_nb, const std::string &err_message, User &sender)
{
        std::string err = ":" + this->getName() + " " + err_nb + " " + sender.getNickname() + " " + err_message + "\r\n";
		this->sendResponse(sender.getFD(), err);
		return ;
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
