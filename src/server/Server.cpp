/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:15:28 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/15 16:45:48 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"


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
        sendResponse(user.getFD(), ":IRCSERV 462 * :You may not register\r\n");
            return;
    }

    if (arguments == _password)
    {
        user.setHasPass(true);
        std::cout << "PASS correct! hasPass = " << user.hasPass() << std::endl;
            return;
     }
    else
        sendResponse(user.getFD(), ":ircserv 464 * :Password incorrect\r\n");
}
// 464 is the IRC numeric for ERR_PASSWDMISMATCH


std::string Server::getName() const
{
    return("ircserv");
}


void Server::sendError(const std::string &err_nb, const std::string &err_message, User &sender)
{
        std::string err = ":" + this->getName() + " " + err_nb + " " + sender.getNickname() + " " + err_message + "\r\n";
		this->sendResponse(sender.getFD(), err);
		return ;
}


void Server::checkRegistration(User &user)
{
    if(!(user.hasPass() && user.hasNick() && user.hasUser() && !user.isRegistered()))
        return ;

    user.setRegistered(true);

    std::string welcome = ":" + this->getName() + " 001 " + user.getNickname() + " :Welcome to the Internet Relay Network\r\n";
	this->sendResponse(user.getFD(), welcome);

	std::string host = ":" + this->getName() + " 002 " + user.getNickname() + " :Your host is ircserv, running version 1.0\r\n";
	this->sendResponse(user.getFD(), host);

	std::string created = ":" + this->getName() + " 003 " + user.getNickname() + " :This server was created today\r\n";
	this->sendResponse(user.getFD(), created);

	std::string info = ":" + this->getName() + " 004 " + user.getNickname() + " " + this->getName() + " 1.0 o itkol\r\n";
	this->sendResponse(user.getFD(), info);

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
